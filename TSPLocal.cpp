//
// Created by bazyli on 24.10.18.
//

#include <climits>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "TSPLocal.h"

/**
 * Solves TSP using greedy algorithm.
 * Greedy algorithm means choosing shortest possible way at every stop.
 * Used as starting path for local search algorithms.
 *
 * @return The greedy solution.
 */
Path TSPLocal::solveGreedy() {
    Path path = Path(size + 1);
    path.setPoint(0, 0);
    path.setPoint(size, 0);

    // For each stop in the path
    for (int i = 1; i < size; ++i) {
        int prev = path.getPoint(i - 1);
        int minDist = INT_MAX;

        // Check all connections to different cities
        for (int j = 1; j < size; ++j) {
            // Skip iteration if this city is already in path
            if (path.inPath(j, i)) continue;

            // If this distance is lower than current minimum set it as minimum
            if (distance[prev][j] < minDist) {
                minDist = distance[prev][j];
                path.setPoint(i, j);
            }
        }
    }

    path.setDistance(dist(path));

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
Path TSPLocal::minNeighbour(Path path, int **tabu, int cadence) {
    // Current minimum neighbour path
    Path minNeigh = Path(size + 1);
    minNeigh.setDistance(INT_MAX);
    // Indexes of swap of the best neighbour
    pair<int, int> bestSwap;

    // Iterate through all possible city swaps (neighbours)
    for (int i = 1; i < size - 1; ++i) {
        for (int j = 1; j < size - 1; ++j) {
            // If this swap is tabu skip it
            if (tabu[i][j]) continue;

            // Perform the swap
            Path curNeigh = path;
            curNeigh.swap(i, j);
            curNeigh.setDistance(dist(curNeigh));

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
void TSPLocal::updateTabu(int **tabu) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (tabu[i][j]) --tabu[i][j];
        }
    }
}

/**
 * Solves TSP using tabu search.
 *
 * @return The resulting path.
 */
Path TSPLocal::solveTabu() {
    if (size <= 0 || distance == nullptr) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Number of iterations
    int iterations = 3000;
    int cadence = 5;

    // Working path
    Path curPath = solveGreedy();
    // Current minimum path
    Path minPath = curPath;
    // Tabu list
    auto **tabu = new int*[size];
    for (int i = 0; i < size; ++i) {
        tabu[i] = new int[size];
        fill(tabu[i], tabu[i] + size, 0);
    }

    // Try specified number of times
    for (int i = 0; i < iterations; ++i) {
        // Find best neighbour of current path
        curPath = minNeighbour(curPath, tabu, cadence);

        // If neighbour is better set it as current minimum
        if (curPath.getDistance() < minPath.getDistance()) {
            minPath = curPath;
        }

        // Update tabu list
        updateTabu(tabu);
    }

    return minPath;
}
