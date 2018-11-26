//
// Created by bazyli on 24.10.18.
//

#include <climits>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <random>
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
    tabu = nullptr;
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
 * Generates random path starting and ending in 0 using Fisher-Yates algorithm.
 *
 * @return Random path.
 */
Path TSPTabuSolver::randomPath() {
    // Path with city number same as its index and return to 0
    Path path(tsp.getSize() + 1);
    path.setPoint(tsp.getSize(), 0);
    for (int i = 0; i < tsp.getSize(); ++i) {
        path.setPoint(i, i);
    }

    default_random_engine r(random_device{}());

    // Shuffle using Fisher-Yates omitting first and last index
    for (int i = 1; i < tsp.getSize() - 2; ++i) {
        uniform_int_distribution<int> range(i, tsp.getSize() - 1);
        path.swap(i, range(r));
    }

    path.setDistance(tsp.pathDist(path));

    return path;
}

/**
 * Performs chosen 2-city move on given path to obtain paths' neighbour.
 *
 * @param path Path to perform the move on.
 * @param x First city.
 * @param y Second city.
 */
void TSPTabuSolver::move(Path &path, int x, int y) {
    switch (neighbourhoodType) {
        case 0:
            path.swap(x, y);
            break;
        case 1:
            path.insert(x, y);
            break;
        case 2:
            path.invert(x, y);
            break;
        default:
            break;
    }
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
    pair<int, int> bestMove;

    // Iterate through all possible 2-city moves
    for (int i = 1; i < tsp.getSize() - 1; ++i) {
        for (int j = 1; j < tsp.getSize() - 1; ++j) {
            // If this move is tabu skip it
            if (tabu[i][j]) continue;

            // Perform the move
            Path curNeigh = path;
            move(curNeigh, i, j);
            curNeigh.setDistance(tsp.pathDist(curNeigh));

            // If resulting path is better than current minimum set it as minimum and save move indexes
            if (curNeigh.getDistance() < minNeigh.getDistance()) {
                minNeigh = curNeigh;
                bestMove = {i, j};
            }
        }
    }

    // Tabu performed swap
    tabu[bestMove.first][bestMove.second] = cadence;
    tabu[bestMove.second][bestMove.first] = cadence;

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

    // Counter of solutions that aren't better than current minimum
    int incorrectCount = 0;
    // Try specified number of times
    for (int i = 0; i < iterations; ++i) {
        // Find best neighbour of current path
        curPath = minNeighbour(curPath);

        // If neighbour is better set it as current minimum
        if (curPath.getDistance() < minPath.getDistance()) {
            minPath = curPath;
        } else {
            // Otherwise increment incorrect counter
            ++incorrectCount;

            // If count of incorrect solutions exceeds the threshold then restart with random path
            if (incorrectThreshold && incorrectCount >= incorrectThreshold) {
                curPath = randomPath();
                clean();
                init();
                incorrectCount = 0;
            }
        }

        // Update tabu list
        updateTabu();
    }

    return minPath;
}

/**
 * Sets problem instance to the given one and performs initialization.
 *
 * @param tsp TSP instance.
 */
void TSPTabuSolver::setTsp(TSP tsp) {
    clean();
    TSPSolver::setTsp(tsp);
    init();
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

/**
 * Sets the neighbourhood type:
 * 0 - swap
 * 1 - insert
 * 2 - invert
 *
 * @param type Neighbourhood type.
 */
void TSPTabuSolver::setNeighbourhoodType(int type) {
    TSPTabuSolver::neighbourhoodType = type;
}

/**
 * Sets incorrect iterations threshold.
 *
 * @param threshold
 */
void TSPTabuSolver::setIncorrectThreshold(int threshold) {
    TSPTabuSolver::incorrectThreshold = threshold;
}
