#ifndef KAKUROBOARD_H
#define KAKUROBOARD_H

#include <vector>
#include <iostream>

class KakuroBoard {
private:
    int rows, cols;
    std::vector<std::vector<int>> board;

public:
    KakuroBoard(int r, int c);
    void randomize();
    void mutate();
    KakuroBoard crossover(const KakuroBoard& other) const;
    bool isValid() const;
    int countSolutions() const;
    void print(std::ostream& out) const;
    friend void saveBestBoard(const KakuroBoard& board);
};

void saveBestBoard(const KakuroBoard& board);

#endif // KAKUROBOARD_H
