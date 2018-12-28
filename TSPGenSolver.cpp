//
// Created by bazyli on 28.12.18.
//

#include "TSPGenSolver.h"

/**
 * Solves TSP using Genetic Algorithm.
 *
 * @return Best found path.
 */
Path TSPGenSolver::solve() {
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
