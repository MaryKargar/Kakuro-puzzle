#include "EvolutionaryAlgorithm.h"
#include <iostream>

int main() {
    int rows, cols;
    std::cout << "Enter board size (rows and columns, e.g., 5 5): ";
    std::cin >> rows >> cols;

    if (rows < 5 || rows > 20 || cols < 5 || cols > 20) {
        std::cerr << "Invalid board size. Please choose dimensions between 5x5 and 20x20.\n";
        return 1;
    }

    std::cout << "Starting the evolutionary algorithm for Kakuro...\n";
    evolve(rows, cols);
    std::cout << "Evolution complete. Best board saved to 'best_kakuro_board.txt'.\n";

    return 0;
}
