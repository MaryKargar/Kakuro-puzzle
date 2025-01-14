#include "KakuroBoard.h"
#include <cstdlib>
#include <ctime>
#include <functional>
#include <fstream>

KakuroBoard::KakuroBoard(int r, int c) : rows(r), cols(c), board(r, std::vector<int>(c, 0)) {
    std::srand(std::time(nullptr));
}

void KakuroBoard::randomize() {
    board.assign(rows, std::vector<int>(cols, 0));
    for (int r = 0; r < rows; ++r) {
        int hintCol = std::rand() % cols;
        board[r][hintCol] = -1;
    }
    for (int c = 0; c < cols; ++c) {
        int hintRow = std::rand() % rows;
        board[hintRow][c] = -1;
    }
}

void KakuroBoard::mutate() {
    int r = std::rand() % rows;
    int c = std::rand() % cols;
    board[r][c] = (board[r][c] == -1) ? 0 : -1;
}

KakuroBoard KakuroBoard::crossover(const KakuroBoard& other) const {
    KakuroBoard child(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            child.board[i][j] = (std::rand() % 2 == 0) ? board[i][j] : other.board[i][j];
        }
    }
    return child;
}

bool KakuroBoard::isValid() const {
    for (int r = 0; r < rows; ++r) {
        bool hasHint = false;
        for (int c = 0; c < cols; ++c) {
            if (board[r][c] == -1) {
                hasHint = true;
                break;
            }
        }
        if (!hasHint) return false;
    }
    for (int c = 0; c < cols; ++c) {
        bool hasHint = false;
        for (int r = 0; r < rows; ++r) {
            if (board[r][c] == -1) {
                hasHint = true;
                break;
            }
        }
        if (!hasHint) return false;
    }
    return true;
}

int KakuroBoard::countSolutions() const {
    int solutionCount = 0;
    std::vector<std::vector<int>> tempBoard = board;

    std::function<bool(int, int)> solve = [&](int r, int c) -> bool {
        if (r == rows) {
            solutionCount++;
            return solutionCount > 1;
        }

        int nextR = (c == cols - 1) ? r + 1 : r;
        int nextC = (c == cols - 1) ? 0 : c + 1;

        if (tempBoard[r][c] != 0) {
            return solve(nextR, nextC);
        }

        for (int num = 1; num <= 9; ++num) {
            tempBoard[r][c] = num;
            if (solve(nextR, nextC)) return true;
            tempBoard[r][c] = 0;
        }

        return false;
    };

    solve(0, 0);
    return solutionCount;
}

void KakuroBoard::print(std::ostream& out) const {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            out << (cell == -1 ? "H" : (cell == 0 ? "." : "X")) << " ";
        }
        out << "\n";
    }
}

void saveBestBoard(const KakuroBoard& board) {
    std::ofstream outFile("best_kakuro_board.txt");
    board.print(outFile);
    outFile.close();
}
