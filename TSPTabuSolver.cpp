//
// Created by bazyli on 24.10.18.
//

#include <climits>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "TSPTabuSolver.h"

/**
 * Initializes fields.
 */
void TSPTabuSolver::init() {
    tabu = new int*[tsp.getSize()];
    for (int i = 0; i < tsp.getSize(); ++i) {
        tabu[i] = new int[tsp.getSize()];
        fill(tabu[i], tabu[i] + tsp.getSize(), 0);
    }
}

/**
 * Frees up memory.
 */
void TSPTabuSolver::clean() {
    if (tabu) {
        for (int i = 0; i < tsp.getSize(); ++i) {
            delete[] tabu[i];
        }
        delete[] tabu;
    }
}

/**
 * Creates solver for given TSP instance.
 *
 * @param tsp TSP instance.
 */
TSPTabuSolver::TSPTabuSolver(TSP tsp) : TSPSolver(tsp) {
    init();
}

/**
 * Frees up memory on object destruction.
 */
TSPTabuSolver::~TSPTabuSolver() {
    clean();
}

/**
 * Sets problem instance to the given one and performs initialization.
 *
 * @param tsp TSP instance.
 */
void TSPTabuSolver::setTsp(TSP tsp) {
    TSPSolver::setTsp(tsp);
    init();
}

/**
 * Solves TSP using greedy algorithm.
 * Greedy algorithm means choosing shortest possible way at every stop.
 * Used as starting path for local search algorithms.
 *
 * @return The greedy solution.
 */
Path TSPTabuSolver::solveGreedy() {
    Path path = Path(tsp.getSize() + 1);
    path.setPoint(0, 0);
    path.setPoint(tsp.getSize(), 0);

    // For each stop in the path
    for (int i = 1; i < tsp.getSize(); ++i) {
        int prev = path.getPoint(i - 1);
        int minDist = INT_MAX;

        // Check all connections to different cities
        for (int j = 1; j < tsp.getSize(); ++j) {
            // Skip iteration if this city is already in path
            if (path.inPath(j, i)) continue;

            // If this distance is lower than current minimum set it as minimum
            if (tsp.dist(prev, j) < minDist) {
                minDist = tsp.dist(prev, j);
                path.setPoint(i, j);
            }
        }
    }

    path.setDistance(tsp.pathDist(path));

    return path;
}

/**
 * Returns best neighbour path for given path. Used in tabu search algorithm.
 *
 * @param path Path of which neighbours will be searched.
 * @param tabu Pointer to tabu list.
 * @param cadence Tabu cadence.
 * @return Best neighbour path of given path.
 */
Path TSPTabuSolver::minNeighbour(Path path) {
    // Current minimum neighbour path
    Path minNeigh = Path(tsp.getSize() + 1);
    minNeigh.setDistance(INT_MAX);
    // Indexes of swap of the best neighbour
    pair<int, int> bestSwap;

    // Iterate through all possible city swaps (neighbours)
    for (int i = 1; i < tsp.getSize() - 1; ++i) {
        for (int j = 1; j < tsp.getSize() - 1; ++j) {
            // If this swap is tabu skip it
            if (tabu[i][j]) continue;

            // Perform the swap
            Path curNeigh = path;
            curNeigh.swap(i, j);
            curNeigh.setDistance(tsp.pathDist(curNeigh));

            // If resulting path is better than current minimum set it as minimum and save swap indexes
            if (curNeigh.getDistance() < minNeigh.getDistance()) {
                minNeigh = curNeigh;
                bestSwap = {i, j};
            }
        }
    }

    // Tabu performed swap
    tabu[bestSwap.first][bestSwap.second] = cadence;

    return minNeigh;
}

/**
 * Updates tabu list by decrementing all non-zero entries.
 *
 * @param tabu Pointer to the tabu list.
 */
void TSPTabuSolver::updateTabu() {
    for (int i = 0; i < tsp.getSize(); ++i) {
        for (int j = 0; j < tsp.getSize(); ++j) {
            if (tabu[i][j]) --tabu[i][j];
        }
    }
}

/**
 * Solves TSP using tabu search.
 *
 * @return The resulting path.
 */
Path TSPTabuSolver::solve() {
    if (tsp.empty()) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Working path
    Path curPath = solveGreedy();
    // Current minimum path
    Path minPath = curPath;

    // Try specified number of times
    for (int i = 0; i < iterations; ++i) {
        // Find best neighbour of current path
        curPath = minNeighbour(curPath);

        // If neighbour is better set it as current minimum
        if (curPath.getDistance() < minPath.getDistance()) {
            minPath = curPath;
        }

        // Update tabu list
        updateTabu();
    }

    return minPath;
}

/**
 * Sets number of neighbour looking cycles.
 *
 * @param iterations Number of iterations.
 */
void TSPTabuSolver::setIterations(int iterations) {
    TSPTabuSolver::iterations = iterations;
}

/**
 * Sets tabu cadence.
 *
 * @param cadence Tabu cadence.
 */
void TSPTabuSolver::setCadence(int cadence) {
    TSPTabuSolver::cadence = cadence;
}
