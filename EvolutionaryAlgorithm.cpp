#include "EvolutionaryAlgorithm.h"
#include "KakuroBoard.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstdlib>

const int POPULATION_SIZE = 150;
const int MAX_GENERATIONS = 100;
const double MUTATION_RATE = 0.3;

double fitness(const KakuroBoard& board) {
    if (!board.isValid()) return -100.0;
    int solutions = board.countSolutions();
    if (solutions == 1) return 1000.0;
    return -500.0 * solutions;
}

void evolve(int rows, int cols) {
    std::vector<KakuroBoard> population(POPULATION_SIZE, KakuroBoard(rows, cols));
    for (auto& board : population) board.randomize();

    for (int gen = 0; gen < MAX_GENERATIONS; ++gen) {
        std::vector<std::pair<double, KakuroBoard>> scoredPopulation;
        for (auto& board : population) {
            scoredPopulation.emplace_back(fitness(board), board);
        }

        std::sort(scoredPopulation.begin(), scoredPopulation.end(),
                  [](const auto& a, const auto& b) { return a.first > b.first; });

        if (scoredPopulation[0].first == 1000.0) {
            std::cout << "Unique solution found at generation " << gen + 1 << "!\n";
            saveBestBoard(scoredPopulation[0].second);
            return;
        }

        population.clear();
        for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
            population.push_back(scoredPopulation[i].second);
        }

        while (population.size() < POPULATION_SIZE) {
            int p1 = std::rand() % (POPULATION_SIZE / 2);
            int p2 = std::rand() % (POPULATION_SIZE / 2);
            population.push_back(population[p1].crossover(population[p2]));
        }

        for (auto& board : population) {
            if ((std::rand() % 100) < MUTATION_RATE * 100) board.mutate();
        }

        std::cout << "Generation " << gen + 1 << " completed.\n";
    }

    saveBestBoard(population[0]);
}
