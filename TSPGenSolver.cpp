//
// Created by bazyli on 28.12.18.
//

#include <random>
#include <stdexcept>
#include <algorithm>
#include "TSPGenSolver.h"

/**
 * Creates solver and calls init.
 */
TSPGenSolver::TSPGenSolver() : TSPSolver() {
    init();
}

/**
 * Creates solver with TSP instance and calls init.
 *
 * @param tsp TSP instance.
 */
TSPGenSolver::TSPGenSolver(TSP tsp) : TSPSolver(tsp) {
    init();
}

/**
 * Calls clean function.
 */
TSPGenSolver::~TSPGenSolver() {
    clean();
}

/**
 * Initializes population array.
 */
void TSPGenSolver::init() {
    population = new Path[populationSize];
}

/**
 * Frees up memory by deleting arrays.
 */
void TSPGenSolver::clean() {
    delete[] population;
    population = nullptr;
}

/**
 * Sorts population in ascending order by path length.
 */
void TSPGenSolver::sortPopulation() {
    std::sort(population, population + populationSize, [](const auto &l, const auto &r){
        return l.getDistance() < r.getDistance();
    });
}

/**
 * Fills population with random paths.
 */
void TSPGenSolver::initPopulation() {
    for (int i = 0; i < populationSize; ++i) {
        population[i].random();
        population[i].setDistance(tsp.pathDist(population[i]));
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

    return Path();
}

/**
 * Sets size of the single population.
 *
 * @param populationSize Size of the population.
 */
void TSPGenSolver::setPopulationSize(int populationSize) {
    TSPGenSolver::populationSize = populationSize;
    clean();
    init();
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
