// Random Generator
#include <iostream>
#include <vector>
#include <array>
#include <iomanip> // For std::setw
#include <ga/GA2DArrayGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/garandom.h>
#include <random>
#include <map>
#include <set>

std::random_device rd;
std::mt19937 gen(rd());

int BOARD_SIZE; // Size of the board
std::map<int, int> rowSums; // Row sum constraints
std::map<int, int> colSums; // Column sum constraints

// Function to initialize the Kakuro board with black, sum, and white cells
void initializeBoard(std::vector<std::vector<std::array<int, 3>>>& board) {
    for (size_t r = 0; r < board.size(); ++r) {
        for (size_t c = 0; c < board[r].size(); ++c) {
            if ((r + c) % 5 == 0) { // Example condition for black cells
                board[r][c] = {0, 0, 0}; // Black cell
            } else if (r % 3 == 0 || c % 4 == 0) { // Example condition for sum cells
                board[r][c] = {-1, std::uniform_int_distribution<int>(10, 45)(gen), std::uniform_int_distribution<int>(10, 45)(gen)}; // Sum cell
            } else {
                board[r][c] = {1, 0, 0}; // White cell
            }
        }
    }
}

// Function to print the board in the desired format
void plotBoard(const std::vector<std::vector<std::array<int, 3>>>& outBoard, bool empty) {
    std::string betweenline((1 + 6 * (BOARD_SIZE)), '-'); // Divider line
    std::cout << betweenline << std::endl;
    for (size_t r = 0; r < outBoard.size(); ++r) {
        std::cout << "|";
        for (size_t c = 0; c < outBoard[r].size(); ++c) {
            if (outBoard[r][c][0] == 0) { // Black cell
                std::cout << "XXXXX";
            } else if (outBoard[r][c][0] > 0) { // White cell (number or empty)
                if (empty) {
                    std::cout << "     "; // Empty cell
                } else {
                    std::cout << "  " << outBoard[r][c][1] << "  "; // Filled cell
                }
            } else { // Sum cell
                std::cout << std::setw(2) << outBoard[r][c][1] << "\\" << std::setw(2) << outBoard[r][c][2];
            }
            std::cout << "|";
        }
        std::cout << std::endl << betweenline << std::endl;
    }
}

// Function to print the board (both empty and filled)
void printBoard(GAGenome& g) {
    auto& genome = (GA2DArrayGenome<int>&)g;

    // Initialize the board structure
    std::vector<std::vector<std::array<int, 3>>> outBoard(BOARD_SIZE, std::vector<std::array<int, 3>>(BOARD_SIZE));
    initializeBoard(outBoard); // Custom initialization logic

    // Fill white cells with genome values
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (outBoard[r][c][0] == 1) { // Only fill white cells
                outBoard[r][c][1] = genome.gene(r, c);
            }
        }
    }

    std::cout << "Empty Board: " << std::endl;
    plotBoard(outBoard, true); // Print the empty board

    std::cout << "Filled Board: " << std::endl;
    plotBoard(outBoard, false); // Print the filled board
}

// Objective function for the genetic algorithm
float objective(GAGenome& g) {
    auto& genome = (GA2DArrayGenome<int>&)g;
    float fitness = 0.0f;

    // Reward for uniqueness in rows and columns
    for (int r = 0; r < BOARD_SIZE; ++r) {
        std::set<int> rowValues;
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int value = genome.gene(r, c);
            if (value > 0) {
                rowValues.insert(value);
            }
        }
        fitness += rowValues.size();
    }

    for (int c = 0; c < BOARD_SIZE; ++c) {
        std::set<int> colValues;
        for (int r = 0; r < BOARD_SIZE; ++r) {
            int value = genome.gene(r, c);
            if (value > 0) {
                colValues.insert(value);
            }
        }
        fitness += colValues.size();
    }

    // Reward for correct sums
    for (int r = 0; r < BOARD_SIZE; ++r) {
        int rowSum = 0;
        for (int c = 0; c < BOARD_SIZE; ++c) {
            rowSum += genome.gene(r, c);
        }
        if (rowSums.count(r) && rowSums[r] == rowSum) {
            fitness += 10.0f; // Reward for correct row sum
        }
    }

    for (int c = 0; c < BOARD_SIZE; ++c) {
        int colSum = 0;
        for (int r = 0; r < BOARD_SIZE; ++r) {
            colSum += genome.gene(r, c);
        }
        if (colSums.count(c) && colSums[c] == colSum) {
            fitness += 10.0f; // Reward for correct column sum
        }
    }

    return fitness;
}

// Function to solve the board using backtracking and count solutions
bool solveKakuro(std::vector<std::vector<int>>& board, int row, int col, int& solutionCount) {
    if (row == BOARD_SIZE) {
        solutionCount++;
        std::cout << "Solution #" << solutionCount << " found." << std::endl;
        return false; // Continue exploring to count all solutions
    }

    int nextRow = (col == BOARD_SIZE - 1) ? row + 1 : row;
    int nextCol = (col == BOARD_SIZE - 1) ? 0 : col + 1;

    if (board[row][col] != 0) { // Skip pre-filled cells
        return solveKakuro(board, nextRow, nextCol, solutionCount);
    }

    for (int num = 1; num <= 9; ++num) {
        board[row][col] = num;
        if (objective((GAGenome&)board)) { // Check if the board is valid
            solveKakuro(board, nextRow, nextCol, solutionCount);
        }
        board[row][col] = 0; // Backtrack
    }

    return false;
}

// Function to check if the solution is unique
bool isUniqueSolution(const std::vector<std::vector<int>>& initialBoard) {
    std::vector<std::vector<int>> board = initialBoard;
    int solutionCount = 0;
    solveKakuro(board, 0, 0, solutionCount);
    return solutionCount == 1;
}

// Custom initializer function
void initializer(GAGenome& g) {
    auto& genome = (GA2DArrayGenome<int>&)g;

    std::uniform_int_distribution<int> dist(1, 9); // Generate values between 1 and 9
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            genome.gene(r, c, dist(gen)); // Assign random values
        }
    }
}

// Main function
int main() {
    int rows;
    std::cout << "Enter board size (5-20): ";
    std::cin >> rows;

    if (rows < 5 || rows > 20) {
        std::cerr << "Invalid board size. Please enter a size between 5 and 20." << std::endl;
        return 1;
    }

    BOARD_SIZE = rows; // Set board size

    // Define the genome
    GA2DArrayGenome<int> genome(BOARD_SIZE, BOARD_SIZE, objective);
    genome.initializer(initializer); // Attach the initializer

    // Set up the genetic algorithm
    GASimpleGA ga(genome);
    ga.populationSize(50);
    ga.nGenerations(1000);
    ga.pMutation(0.1f);
    ga.pCrossover(0.9f);

    ga.initialize();

    // Run the genetic algorithm
    while (!ga.done()) {
        ga.step();
        if (ga.generation() % 100 == 0) {
            std::cout << "Generation: " << ga.generation()
                      << ", Best fitness: " << ga.statistics().bestIndividual().score() << std::endl;
        }
    }

    // Retrieve the best solution
    GAGenome& bestGenome = (GA2DArrayGenome<int>&)ga.statistics().bestIndividual();

    // Print the solution
    printBoard(bestGenome);

    // Check if the solution is unique
    std::vector<std::vector<int>> solvedBoard(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            solvedBoard[r][c] = ((GA2DArrayGenome<int>&)bestGenome).gene(r, c);
        }
    }

    if (isUniqueSolution(solvedBoard)) {
        std::cout << "The solution is unique!" << std::endl;
    } else {
        std::cout << "The solution is not unique!" << std::endl;
    }

    return 0;
}
