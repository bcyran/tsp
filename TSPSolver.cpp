//
// Created by bazyli on 25.10.18.
//

#include "TSPSolver.h"

/**
 * Create empty solver instance.
 */
TSPSolver::TSPSolver() = default;

/**
 * Create solver for given TSP instance.
 *
 * @param tsp TSP instance.
 */
TSPSolver::TSPSolver(TSP tsp) : tsp(tsp) {}

/**
 * Set problem instance to the given one.
 *
 * @param tsp TSP instance.
 */
void TSPSolver::setTsp(TSP tsp) {
    TSPSolver::tsp = tsp;
}

/**
 * Helper to generate new random TSP inside of the solver.
 *
 * @param size Size of the problem.
 */
void TSPSolver::random(int size) {
    tsp.random(size);
}
