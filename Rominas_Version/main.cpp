#include <iostream>
#include <random>
#include <array>
#include <ga/GA2DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/std_stream.h>
#include <ga/garandom.h>
// First Row and Column are always black fields (Space for the sums) --> make board in algorithm smaller

// Random
std::random_device rd;
std::mt19937 gen(rd());

const int BOARD_SIZE=4;   // real BoardSize = BOARD_SIZE-1 --> Row+Column with Idx 0 are always black fields
const int POPULATION_SIZE = 500;
const int MAX_GENERATIONS = 2000;

float objective(GAGenome& g) {
    auto& genome = (GA2DArrayGenome<int>&)g;

    // In Fitnessfunction two constrains are tested:
    // Is Number in its Scope Unique or not
    // Right and under of a black field it is not allowed to have only one white field (0 or >= 2)
    // todo: Check if it is a unique solution
    float fitness = BOARD_SIZE*BOARD_SIZE*BOARD_SIZE;
    float count_notunique = 0;
    float count_single = 0;
    float count_whitefields = 0;
    for (size_t r = 0; r < BOARD_SIZE; r++) {
        size_t pos = -1;    // last black field
        for (size_t c = 0; c < BOARD_SIZE; c++) {
            if (genome.gene(r, c) == 0) {
                // black field
                if (c-pos == 2) {
                    // Only one white field between two black
                    count_single++;
                }
                pos = c;
            } else {
                count_whitefields ++;
                if (pos+1!=c) {
                    for (size_t i=pos+1; i<c; ++i) {
                        if (genome.gene(r, i) == genome.gene(r, c)) {
                            count_notunique++;
                        }
                    }
                }
            }
        }
        // check if last field is single white field
        if (genome.gene(r, BOARD_SIZE-1)!=0 && genome.gene(r, BOARD_SIZE-2)==0) {
            count_single++;
        }
    }
    for (size_t c = 0; c < BOARD_SIZE; c++) {
        size_t pos = -1;    // last black field
        for (size_t r = 0; r < BOARD_SIZE; r++) {
            if (genome.gene(r, c) == 0) {
                // black field
                if (r-pos == 2) {
                    // Only one white field between two black
                    count_single++;
                }
                pos = r;
            } else if (pos+1!=r) {
                for (size_t i=pos+1; i<r; ++i) {
                    if (genome.gene(i, c) == genome.gene(r, c)) {
                        count_notunique++;
                    }
                }
            }
        }
        // check if last field is single white field
        if (genome.gene(BOARD_SIZE-1, c)!=0 && genome.gene(BOARD_SIZE-2, c)==0) {
            count_single++;
        }
    }
    fitness = fitness-count_notunique-count_single+(count_whitefields/2);
    //std::cout << fitness << std::endl;
    return fitness;
}

// Initializer
void initializer(GAGenome& g) {
    GA2DArrayGenome<int>&  genome = (GA2DArrayGenome<int>&)g;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            genome.gene(r, c, std::uniform_int_distribution<int>(0, 9)(gen));
            /*if (r==0 || c==0) {
                // Column and Row with idx 0 have to be black
                genome.gene(r, c, 0);
            } else {
                genome.gene(r, c, std::uniform_int_distribution<int>(0, 9)(gen));
            }*/
        }
    }
}

// Mutator
int mutator(GAGenome& g, float p) {
    auto& genome = (GA2DArrayGenome<int>&)g;

    int nMutations = 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GAFlipCoin(p)) {
                int othR = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);
                int othC = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);
                int temp = genome.gene(othR, othC);
                genome.gene(othR, othC, genome.gene(r,c));
                genome.gene(r, c, temp);
                nMutations++;
                //std::cout << genome.gene(r,c) << ";" << genome.gene(othR, othC) << std::endl;
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

        int cutR = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);
        int cutC = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);

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
        return 2;
    } else if (c1) {
        auto& child = (GA2DArrayGenome<int>&)*c1;
        int cutR = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);
        int cutC = std::uniform_int_distribution<int>(0, BOARD_SIZE-1)(gen);

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
        return 1;
    } else {
        return 0;
    }
}

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

void printBoard(GAGenome& g) {
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
    for (auto [r,c,val]:sumValues) {
        outBoard[r+1][c+1][2] = val;
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
    for (auto [r,c,val]:sumValues) {
        outBoard[r+1][c+1][1] = val;
    }

    std::cout << "Empty Board: " << std::endl;
    plotBoard(outBoard, true);

    std::cout << std::endl << "Filled Board: " << std::endl;
    plotBoard(outBoard, false);

}

int main() {
    GA2DArrayGenome<int> genome(BOARD_SIZE, BOARD_SIZE, objective);
    genome.initializer(initializer);
    genome.mutator(mutator);
    genome.crossover(crossover);

    GASimpleGA ga(genome);
    ga.populationSize(POPULATION_SIZE);
    ga.nGenerations(MAX_GENERATIONS);
    ga.pMutation(0.05);
    ga.pCrossover(0.9);
    ga.evolve();

    GAGenome &bestGenome = (GA2DArrayGenome<int> &) ga.statistics().bestIndividual();
    std::cout << "Best solution found: " << std::endl;
    printBoard(bestGenome);

    return 0;
};