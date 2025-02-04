#include <algorithm>
#include <iostream>
#include <random>
#include <array>
#include <tuple>
#include <ga/GA2DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/std_stream.h>
#include <ga/garandom.h>
#include "kakurosolver/kakurosolver.h"
#include <bits/stdc++.h>

// Random
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 100);

int BOARD_SIZE;   // real BoardSize = BOARD_SIZE-1 --> Row+Column with Idx 0 are always black fields
constexpr int POPULATION_SIZE = 50;
constexpr int MAX_GENERATIONS = 500;
constexpr float MUTATION_RATE = 0.8;
constexpr float CROSSOVER_RATE = 0.6;
constexpr float BLACK_PERC = 20; // Percentage of black fields (without Row and Column with Idx 0

void plotBoard(const std::vector<std::vector<std::array<int,3>>>& outBoard, bool empty) {
    std::string betweenline((1+6*(BOARD_SIZE+1)),'-');
    std::cout << betweenline << std::endl;
    for (const auto& row:outBoard) {
        std::cout << "|";
        for (const auto& col: row) {
            if (col[0]==0) {
                // Black Field
                if (col[1] == 0 && col[2] == 0) {
                    std::cout << "XXXXX";
                } else {
                    if (col[1] != 0) {
                        if (col[1]<10) std::cout << " " << col[1];
                        else std::cout << col[1];
                    } else std::cout << "XX";
                    std::cout << "\\";
                    if (col[2] != 0) {
                        if (col[2]<10) std::cout << " " << col[2];
                        else std::cout << col[2];
                    } else std::cout << "XX";
                }
            } else {
                if (!empty) std::cout << "  " << col[1] << "  ";
                else std::cout << "     ";
            }
            std::cout << "|";
        }
        std::cout << std::endl;
        std::cout << betweenline << std::endl;
    }
}

std::vector<std::vector<std::array<int,3>>> calcBoard(GAGenome& g) {
    auto& genome = (GA2DArrayGenome<int>&)g;

    std::vector<std::vector<std::array<int,3>>> outBoard; // White or Black, downSum/ResultNumber, rightSum
    // Calculate Sum Values
    std::vector<std::tuple<int,int,int>> sumValues;

    std::vector<std::array<int, 3>> row;
    row.push_back({0,0,0});
    for (int c = 0; c < BOARD_SIZE; c++) {
        row.push_back({0,0,0});
    }
    outBoard.push_back(row);
    for (int r = 0; r < BOARD_SIZE; r++) {
        int pos = -1;
        int val = 0;
        std::vector<std::array<int, 3>> row;
        row.push_back({0,0,0}); // Black Field at Index 0
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (genome.gene(r,c) == 0) {
                if (val != 0) {
                    sumValues.emplace_back(r,pos,val);
                    val = 0;
                    pos = c;
                } else {
                    pos = c;
                }
                row.push_back({0,0,0});
            } else {
                val += genome.gene(r,c);
                row.push_back({1,genome.gene(r,c),0});
            }
        }
        sumValues.emplace_back(r,pos,val);
        outBoard.push_back(row);
    }
    for (auto a:sumValues) {
        outBoard[std::get<0>(a)+1][std::get<1>(a)+1][2] = std::get<2>(a);
    }
    sumValues.clear();
    for (int c = 0; c < BOARD_SIZE; c++) {
        int pos = -1;
        int val = 0;
        for (int r = 0; r < BOARD_SIZE; r++) {
            if (genome.gene(r,c) == 0) {
                if (val != 0) {
                    sumValues.emplace_back(pos,c,val);
                    val = 0;
                    pos = r;
                } else {
                    pos = r;
                }
            } else {
                val += genome.gene(r,c);
            }
        }
        sumValues.emplace_back(pos,c,val);
    }
    for (auto a:sumValues) {
        outBoard[std::get<0>(a)+1][std::get<1>(a)+1][1] = std::get<2>(a);
    }
    return outBoard;
}

void printBoard(GAGenome& g) {
    std::vector<std::vector<std::array<int,3>>> outBoard = calcBoard(g);

    std::cout << "Empty Board: " << std::endl;
    plotBoard(outBoard, true);

    std::cout << std::endl << "Filled Board: " << std::endl;
    plotBoard(outBoard, false);
}


bool isValid(GAGenome& g) {
    auto& genome = (GA2DArrayGenome<int>&)g;

    // For each Row
    for (size_t r = 0; r < BOARD_SIZE; r++) {
        size_t pos = -1;    // last black field
        for (size_t c = 0; c < BOARD_SIZE; c++) {
            if (genome.gene(r, c) == 0) {
                // black field
                if (c-pos == 2 || c-pos > 10)   return false;   // Only one or more than 10 white field between two black
                pos = c;
            } else {
                for (size_t i=pos+1; i<c; ++i) {
                    if (genome.gene(r, i) == genome.gene(r, c)) return false;
                }
            }
        }
        // check if last field is single white field
        if (genome.gene(r,BOARD_SIZE-1)!=0 && genome.gene(r,BOARD_SIZE-2)==0)   return false;
    }
    // For each Column
    for (size_t c = 0; c < BOARD_SIZE; c++) {
        size_t pos = -1;    // last black field
        for (size_t r = 0; r < BOARD_SIZE; r++) {
            if (genome.gene(r, c) == 0) {
                // black field
                if (r-pos == 2) return false;   // Only one white field between two black
                pos = r;
            } else {
                for (size_t i=pos+1; i<r; ++i) {
                    if (genome.gene(i, c) == genome.gene(r, c)) return false;
                }
            }
        }
        // check if last field is single white field
        if (genome.gene(BOARD_SIZE-1, c)!=0 && genome.gene(BOARD_SIZE-2, c)==0) return false;
    }
    return true;
}

int kakuroSolver(GAGenome& g, bool asc=true) {
    // Run kakuroSolver
    vector<int> threads{1, 2, 4, 8, 9, 16};
    auto board = calcBoard(g);
    //printBoard(g);
    int m, n;
    m = board.size();
    n = board[0].size();
    int **matrix = new int *[m]; // rows
    for (int i = 0; i < m; i++) matrix[i] = new int[n]; // cols

    int val;
    const std::array<int, 3> blackWithout = {0, 0, 0};
    for (int r=0; r<board.size();r++) {
        for (int c=0; c<board.size();c++) {
            if (board[r][c] == blackWithout) {
                matrix[r][c] = -1;
            } else if (board[r][c][0] == 0) {
                // Black Field with Sums
                matrix[r][c] = board[r][c][1]*10+board[r][c][2]*1000;
            } else {
                // White Field
                matrix[r][c] = -2;
            }
        }
    }

    int status=2;
    int **sol_mat;
    int empty_cells = convert_sol(matrix, sol_mat, m, n);
    vector<sum> sums = get_sums(matrix, m, n);

    // Serial Solver
    //solution_serial(matrix, sol_mat, sums, m, n, m * n, empty_cells);
    // OpenMP Solver
    solution(matrix, sol_mat, sums, m, n, m * n, empty_cells);

    if (sol_mat == NULL) {
        // No Solution found
        status = 0;
    } else {
        // Check if solution is equal to the solution of the GA
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // Not a unique Solution
                if (sol_mat[i][j] != -1 && sol_mat[i][j] != board[i][j][1]) {
                    status = 1;
                    break;
                }
            }
            if (status == 1) break;
        }
    }
    //print_one_matrix(sol_mat, m, n);
    for (int i = 0; i < n; i++) {
        delete matrix[i];
        delete sol_mat[i];
    }
    delete matrix;
    delete sol_mat;
    return status;
    std::cout<<"Kakuro Solver End"<<std::endl;
}

// Fitnessfunction
float objective(GAGenome& g) {
    auto& genome = (GA2DArrayGenome<int>&)g;
    if (!isValid(g)) return 0;
    int result = kakuroSolver(g);

    if (result==2) {
        // Unique Solution
        return 100;
    }
    if (result == 1) {
        // Valid Board
        return 10;
    }
    return 5;
}

/*
// Fitnessfunction
float objective(GAGenome& g) {

    auto& genome = (GA2DArrayGenome<int>&)g;
    std::vector<std::vector<std::array<int,3>>> board = calcBoard((GA2DArrayGenome<int>&)g);

    float count_black = 0;
    float count_white = 0;
    float count_isolatedWhite = 0;
    set<int> sumsH;
    float count_sumsH = 0;
    set<int> sumsV;
    float count_sumsV = 0;
    float count_duplicates = 0;
    for (int r=0; r<board.size(); r++) {
        for (int c=0; c<board.size(); c++) {
            if (board[r][c][0] == 0) {
                // Black Field
                count_black++;

                // Check Sums - Vertical
                if (board[r][c][1] != 0) {
                    sumsV.insert(board[r][c][1]);
                    count_sumsV++;
                    std::vector<int> numbers;
                    for (int i=1; i<=9; ++i) {
                        if (r+i>=board.size() || board[r+i][c][0] == 0) {
                            break;
                        }
                        // Unique Numbers in Scope
                        if (std::find(numbers.begin(), numbers.end(), board[r+i][c][1]) == numbers.end()) {
                            numbers.push_back(i);
                        } else {
                            count_duplicates++;
                        }
                    }
                }
                // Check Sums - Horizontal
                if (board[r][c][1] != 0) {
                    sumsH.insert(board[r][c][2]);
                    count_sumsH++;
                    std::vector<int> numbers;
                    for (int i=1; i<=9; ++i) {
                        if (c+i>=board.size() || board[r][c+i][0] == 0) {
                            break;
                        }
                        // Unique Numbers in Scope
                        if (std::find(numbers.begin(), numbers.end(), board[r][c+i][1]) == numbers.end()) {
                            numbers.push_back(i);
                        } else {
                            count_duplicates++;
                        }
                    }
                }
                // Count Neighbours
                for (int i=-1; i<=1; i++) {
                    for (int j=-1; j<=1; j++) {
                        if (i==0 && j==0) continue;
                        if (r+i<0 || r+i>=board.size() || c+j<0 || c+j>=board.size()) continue;
                        if (board[r+i][c+j][0] == 0) {
                            count_black++;
                        }
                    }
                }
            } else {
                // White Field
                count_white++;
                if (board[r-1][c][0] == 0 && (r == board.size()-1 || board[r+1][c][0]==0))
                    count_isolatedWhite++;
                if (board[r][c-1][0] == 0 && (c == board.size()-1 || board[r][c+1][0] == 0)) {
                    count_isolatedWhite++;
                }
                // Count Neighbours
                for (int i=-1; i<=1; i++) {
                    for (int j=-1; j<=1; j++) {
                        if (i==0 && j==0) continue;
                        if (r+i<0 || r+i>=board.size() || c+j<0 || c+j>=board.size()) continue;
                        if (board[r+i][c+j][0] == 1) {
                            count_white++;
                        }
                    }
                }
            }
        }
    }

    //float fitness = static_cast<float>(BOARD_SIZE * BOARD_SIZE * BOARD_SIZE);
    float fitness = pow(((float)sumsH.size()/count_sumsH),2)*100 + pow(((float)sumsV.size()/count_sumsV),2)*100;
    fitness += pow((count_black/(count_black+count_white))-0.20,2)*100;
    fitness -= pow(count_isolatedWhite,2);
    fitness -= pow(count_duplicates,2);
    if (isValid(g)) fitness += 1000;
    return fitness;
}
*/

// Initializer
void initializer(GAGenome& g) {
    auto&  genome = (GA2DArrayGenome<int>&)g;

    int count = 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            bool valid = false;
            int value;
            count = 0;  //Restart Boardbuilding if valid number for field cannot be found
            while (!valid && count <= 30) {
                count ++;
                valid = true;
                int randNum = dis(gen);
                if (randNum <= BLACK_PERC) value = 0;   // BLACK_PERC Black Fields
                else value = randNum % 9 + 1;
                // Test if value would create invalid board
                // Check Horizontal
                int count_whitefields = 0;
                if (value > 0) count_whitefields++;
                for (int cc=c-1; cc>=0; cc--) {
                    if (genome.gene(r, cc) == 0)    break;  // Black Field --> Stop loop
                    if (genome.gene(r, cc) == value) {
                        // Value is invalid (must be unique) --> Try again
                        valid = false;
                        break;
                    }
                    count_whitefields++;
                }
                if ((count_whitefields == 1 && value == 0) || count_whitefields > 9 )   valid = false;
                if (c == BOARD_SIZE-1 && value != 0 && count_whitefields == 1) {
                    value = 0;
                }
                if (!valid) continue;

                // Check Vertical
                count_whitefields = 0;
                if (value > 0) count_whitefields++;
                for (int rr=r-1; rr>=0; rr--) {
                    if (genome.gene(rr, c) == 0)    break; // Black Field --> Stop loop
                    if (genome.gene(rr, c) == value) {
                        // Value is invalid (must be unique --> Try again
                        valid = false;
                        break;
                    }
                    count_whitefields++;
                }
                // 2 to 9 white fields can be next to each other
                if ((count_whitefields == 1 && value == 0) || count_whitefields > 9 ) valid = false;
                if (r == BOARD_SIZE-1 && value != 0 && count_whitefields == 1) valid = false;
            }
            genome.gene(r, c, value);
            if (count > 30) break;
        }
        if (count > 30) break;
    }
    if (count > 30) initializer(g);
}


// Mutator
int mutator(GAGenome& g, float p) {
    auto& genome = (GA2DArrayGenome<int>&)g;
    int nMutations = 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GAFlipCoin(p)) {
                int oldvalue  = genome.gene(r, c);

                bool result = false;
                int count = 0;
                while (!result) {
                    if (count>30) {
                        genome.gene(r, c, oldvalue);
                        break;
                    }
                    count++;
                    int value;
                    int randNum = dis(gen);
                    if (randNum <= BLACK_PERC) value = 0;   // BLACK_PERC Black Fields
                    else value = randNum % 9 + 1;
                    genome.gene(r, c, value);
                    if (isValid(genome)) {
                        if (oldvalue != genome.gene(r,c)) nMutations++;
                        result=true;
                    }
                }
            }
        }
    }
    return nMutations;
}


int crossover(const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2) {
    auto& parent1 = (GA2DArrayGenome<int>&)p1;
    auto& parent2 = (GA2DArrayGenome<int>&)p2;

    if (c1 && c2) {
        auto& child1 = (GA2DArrayGenome<int>&)*c1;
        auto& child2 = (GA2DArrayGenome<int>&)*c2;

        bool result = false;
        int count = 0;
        int cutR;
        int cutC;
        while (!result) {
            count ++;
            if (count > 30) return 0;
            cutR = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);
            cutC = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);

            for (int r=0; r<cutR; r++) {
                for (int c=0; c<cutC; c++) {
                    child1.gene(r, c, parent1.gene(r, c));
                    child2.gene(r, c, parent2.gene(r, c));
                }
            }
            for (int r=cutR; r<BOARD_SIZE; r++) {
                for (int c=cutC; c<BOARD_SIZE; c++) {
                    child1.gene(r, c, parent2.gene(r, c));
                    child2.gene(r, c, parent1.gene(r, c));
                }
            }
            if (isValid(child1) && isValid(child2)) result = true;
        }
        if (cutC!=0 && cutR!=0 && cutC!=BOARD_SIZE-1 && cutR!=BOARD_SIZE-1) return 2;
        return 0;
    } else if (c1) {
        auto& child = (GA2DArrayGenome<int>&)*c1;

        int count=0;
        bool result = false;
        int cutR;
        int cutC;
        while (!result) {
            count++;
            if (count > 30) return 0;

            cutR = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);
            cutC = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);

            for (int r=0; r<cutR; r++) {
                for (int c=0; c<cutC; c++) {
                    child.gene(r, c, parent1.gene(r, c));
                }
            }
            for (int r=cutR; r<BOARD_SIZE; r++) {
                for (int c=cutC; c<BOARD_SIZE; c++) {
                    child.gene(r, c, parent2.gene(r, c));
                }
            }
            if (isValid(child)) result = true;
        }
        if (cutC!=0 && cutR!=0 && cutC!=BOARD_SIZE-1 && cutR!=BOARD_SIZE-1) return 1;
        return 0;
    } else {
        return 0;
    }
}



int main() {
    int rows;
    std::cout << "Enter board size (e.g. 5): ";
    std::cin >> rows;

    if (rows < 5 || rows > 20) {
        std::cerr << "Invalid board size. Please choose dimensions between 5x5 and 20x20." << std::endl;
        return 1;
    }
    // First Row and Column are always black fields (->Space for the sums) -->board size in algorithm smaller
    BOARD_SIZE = rows - 1;

    GA2DArrayGenome<int> genome(BOARD_SIZE, BOARD_SIZE, objective);
    genome.initializer(initializer);
    genome.mutator(mutator);
    genome.crossover(crossover);

    GASimpleGA ga(genome);
    ga.populationSize(POPULATION_SIZE);
    ga.nGenerations(MAX_GENERATIONS);
    ga.pMutation(MUTATION_RATE);
    ga.pCrossover(CROSSOVER_RATE);

    ga.initialize();

    bool status = false;
    std::cout << "Evolving..."<<std::endl;
    while(!ga.done()){
        ga.step();
        GAGenome &bestGenome = (GA2DArrayGenome<int> &) ga.statistics().bestIndividual();

        if (bestGenome.score() >= 100) {
            std::cout << "Solution found: " << std::endl;
            printBoard(bestGenome);

            //std::cout << "Fitness: " << bestGenome.score() << std::endl;
            status = true;
            break;
        }

        if(ga.generation() % 1 == 0){
            std::cout << "Generation: " << ga.generation()  << std::endl;
        }
    }
    std::cout << "\n\n";
    if (!status) {
        std::cout << "No Unique Board found! Try again!" << std::endl;
    }
    return 0;
}


