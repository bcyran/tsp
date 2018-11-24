//
// Created by bazyli on 12.11.18.
//

#include <random>
#include <stdexcept>
#include <iostream>
#include "TSPSimAnnealingSolver.h"

/**
 * Generates random path starting and ending in 0 using Fisher-Yates algorithm.
 *
 * @return Random path.
 */
Path TSPSimAnnealingSolver::randomPath() {
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
void TSPSimAnnealingSolver::move(Path &path, int x, int y) {
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
    Path curPath = randomPath();
    // Current minimum path
    Path minPath = curPath;

    // Current temperature
    double temp = initTemp;

    // Loop as long as system temperature is higher than minimum
    while (temp > endTemp) {
        // Execute number of times specified by iterations field
        for (int i = 0; i < iterations; ++i) {
            Path newPath = randomNeighbour(curPath);

            // If new solution is better than minimum set it as min and current
            if (newPath.getDistance() < minPath.getDistance()) {
                minPath = newPath;
                curPath = newPath;
                continue;
            }

            // Difference between current and new distance
            int deltaDist = newPath.getDistance() - curPath.getDistance();

            // If new path is shorter than the current one accept it and go to the next iteration
            if (deltaDist <= 0) {
                curPath = newPath;
                continue;
            }

            // Accept the path depending on calculated acceptance probability
            if (exp(-deltaDist / temp) > randomProb()) {
                curPath = newPath;
            }
        }

        // Decrease the temperature
        temp *= 1 - coolingRate;
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
void TSPSimAnnealingSolver::setNeighbourhoodType(int neighbourhoodType) {
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
