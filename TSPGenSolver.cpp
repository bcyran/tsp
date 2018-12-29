//
// Created by bazyli on 28.12.18.
//

#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "TSPGenSolver.h"

/**
 * Returns random probability, a number between 0 and 1.
 *
 * @return Random probability.
 */
float TSPGenSolver::randomProb() {
    default_random_engine r(random_device{}());
    uniform_real_distribution<float> range(0.0, 1.0);

    return range(r);
}

/**
 * Sorts population in ascending order by path length.
 */
void TSPGenSolver::sortPopulation() {
    std::sort(population.begin(), population.end(), [](const auto &l, const auto &r) {
        return l.getDistance() < r.getDistance();
    });
}

/**
 * Fills population with random paths.
 */
void TSPGenSolver::initPopulation() {
    // Allocate memory for the population
    population.reserve(static_cast<unsigned long>(populationSize));
    // Fill the population
    for (int i = 0; i < populationSize; i++) {
        Path path = Path(tsp.getSize() + 1);
        path.random();
        path.setDistance(tsp.pathDist(path));
        population.push_back(path);
    }
    sortPopulation();
}

/**
 * Fill mating pool with individuals chosen using Roulette Wheel Selection.
 */
void TSPGenSolver::selection() {
    // Allocate memory for the mating pool
    matingPool.reserve(static_cast<unsigned long>(populationSize));
    // Copy elite straight to the mating pool
    matingPool.insert(matingPool.end(), population.begin(), population.begin() + eliteSize);

    unsigned int sum = 0;
    unsigned int cumSum[populationSize];
    float prob[populationSize];

    // Calculate cumulative sum of distances for each individual and total sum
    for (int i = 0; i < populationSize; ++i) {
        sum += population[i].getDistance();
        cumSum[i] = sum;
    }
    // Calculate the cumulative probability that each individual will be NOT selected
    // Probability has to be "reversed" because it's a minimization problem: shorter = better
    for (int i = 0; i < populationSize; ++i) {
        prob[i] = static_cast<float>(cumSum[i]) / sum;
    }

    // Run for each empty slot in mating pool
    for (int i = eliteSize; i < populationSize; ++i) {
        // Spin te roulette
        float roulette = randomProb();
        for (int j = 0; j < populationSize; ++j) {
            if (roulette <= prob[j]) {
                matingPool.push_back(population[j]);
                break;
            }
        }
    }
}

/**
 * Solves TSP using Genetic Algorithm.
 *
 * @return Best found path.
 */
Path TSPGenSolver::solve() {
    if (tsp.empty()) {
        throw runtime_error("Cannot solve empty problem.");
    }

    initPopulation();
    selection();

    return Path();
}

/**
 * Sets size of the single population.
 *
 * @param populationSize Size of the population.
 */
void TSPGenSolver::setPopulationSize(int populationSize) {
    TSPGenSolver::populationSize = populationSize;
}

/**
 * Sets number of individuals carried over to the next generation.
 *
 * @param eliteSize Size of the elite.
 */
void TSPGenSolver::setEliteSize(int eliteSize) {
    TSPGenSolver::eliteSize = eliteSize;
}

/**
 * Sets rate at which mutations occur.
 *
 * @param mutationRate Mutation rate.
 */
void TSPGenSolver::setMutationRate(float mutationRate) {
    TSPGenSolver::mutationRate = mutationRate;
}

/**
 * Set number of generations to evolve.
 *
 * @param generations Number of generations.
 */
void TSPGenSolver::setGenerations(int generations) {
    TSPGenSolver::generations = generations;
}
