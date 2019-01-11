//
// Created by bazyli on 12.11.18.
//

#include <random>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include "TSPSimAnnealingSolver.h"

/**
 * Performs chosen 2-city move on given path to obtain paths' neighbour.
 *
 * @param path Path to perform the move on.
 * @param x First city.
 * @param y Second city.
 */
void TSPSimAnnealingSolver::move(Path &path, int x, int y) {
    switch (neighbourhoodType) {
        case Neighbourhood::SWAP:
            path.swap(x, y);
            break;
        case Neighbourhood::INSERT:
            path.insert(x, y);
            break;
        case Neighbourhood::INVERT:
            path.invert(x, y);
            break;
        default:
            break;
    }
}

/**
 * Returns random neighbour path for given path.
 *
 * @param path Path to find neighbour of.
 * @return Random neighbour.
 */
Path TSPSimAnnealingSolver::randomNeighbour(Path path) {
    default_random_engine r(random_device{}());
    uniform_int_distribution<int> range(1, tsp.getSize() - 1);

    int x, y;

    x = range(r);
    do {
        y = range(r);
    } while (y == x);

    Path neighbour = path;
    move(neighbour, x, y);
    neighbour.setDistance(tsp.pathDist(neighbour));

    return neighbour;
}

/**
 * Returns random probability, a number between 0 and 1.
 *
 * @return Random probability.
 */
double TSPSimAnnealingSolver::randomProb() {
    default_random_engine r(random_device{}());
    uniform_real_distribution<> range(0, 1);

    return range(r);
}

/**
 * Solves TSP using simulated annealing.
 *
 * @return The resulting path.
 */
Path TSPSimAnnealingSolver::solve() {
    if (tsp.empty()) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Working path
    Path curPath(tsp.getSize() + 1);
    curPath.random();
    curPath.setDistance(tsp.pathDist(curPath));
    // Current minimum path
    Path minPath = curPath;

    // Current temperature
    double temp = initTemp;
    auto startTime = chrono::high_resolution_clock::now();

    // Loop as long as system temperature is higher than minimum
    while (true) {
        // Execute number of times specified by iterations field
        for (int i = 0; i < iterations; ++i) {
            Path newPath = randomNeighbour(curPath);

            // Difference between current and new distance
            int deltaDist = newPath.getDistance() - curPath.getDistance();

            // If new path is shorter or equal to current
            if (deltaDist <= 0) {
                // If it's shorter set it as current minimum
                if (deltaDist < 0) {
                    minPath = newPath;
                }

                // Set new path as current path and jump to the next iteration
                curPath = newPath;
            } else if (exp(-deltaDist / temp) > randomProb()) {
                // If path is longer accept it depending on calculated acceptance probability
                curPath = newPath;
            }
        }

        // Decrease the temperature
        temp *= 1 - coolingRate;

        // Stop conditions
        if (runTime) {
            // Calculate execution time and break if it exceeds maximum time
            auto currentTime = chrono::high_resolution_clock::now();
            int elapsedTime = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(currentTime - startTime).count());

            if (elapsedTime > runTime) break;
        } else {
            if (temp < endTemp) break;
        }
    }

    return minPath;
}

/**
 * Sets initial temperature.
 *
 * @param initTemp Initial temperature.
 */
void TSPSimAnnealingSolver::setInitTemp(double initTemp) {
    TSPSimAnnealingSolver::initTemp = initTemp;
}

/**
 * Sets temperature which will stop annealing when reached.
 *
 * @param endTemp Ending temperature.
 */
void TSPSimAnnealingSolver::setEndTemp(double endTemp) {
    TSPSimAnnealingSolver::endTemp = endTemp;
}

/**
 * Sets rate at which system will be cooled.
 *
 * @param coolingRate Cooling rate,
 */
void TSPSimAnnealingSolver::setCoolingRate(double coolingRate) {
    TSPSimAnnealingSolver::coolingRate = coolingRate;
}

/**
 * Sets the neighbourhood type:
 * 0 - swap
 * 1 - insert
 * 2 - invert
 *
 * @param type Neighbourhood type.
 */
void TSPSimAnnealingSolver::setNeighbourhoodType(Neighbourhood neighbourhoodType) {
    TSPSimAnnealingSolver::neighbourhoodType = neighbourhoodType;
}

/**
 * Sets number of iterations in each temperature cycle.
 *
 * @param iterations Number of iterations.
 */
void TSPSimAnnealingSolver::setIterations(int iterations) {
    TSPSimAnnealingSolver::iterations = iterations;
}

/**
 * Sets maximum time of execution in ms.
 *
 * @param runTime Time in ms.
 */
void TSPSimAnnealingSolver::setRunTime(int runTime) {
    TSPSimAnnealingSolver::runTime = runTime;
}
