//
// Created by bazyli on 25.10.18.
//

#include <stdexcept>
#include <climits>
#include "TSPDPSolver.h"

/**
 * Implements recursive Held-Karp algorithm.
 *
 * @param city Current city.
 * @param set Set of visited cities.
 * @param mem Pointer to memoization array.
 * @param fullSet Full set.
 * @return Min path for given input.
 */
int TSPDPSolver::HeldKarp(int city, unsigned int set) {
    // If all nodes are visited
    if (set == FULL_SET) {
        return tsp.dist(city, 0);
    }

    // If this combination of city and set is already memorized.
    if (mem[city][set] != -1) {
        return mem[city][set];
    }

    // Current minimum distance and city
    int minDist = INT_MAX;
    int minCity = -1;

    // Iterate through all unvisited cities
    for (int i = 0; i < tsp.getSize(); ++i) {
        unsigned int mask = 1u << (unsigned) i;

        // If i city wasn't visited
        if (!(set & mask)) {
            // Enter next recursion level with masked current node
            int dist = tsp.dist(city, i) + HeldKarp(i, set | mask);
            // If new distance is shorter than current min set it as min
            if (dist < minDist) {
                minDist = dist;
                minCity = i;
            }
        }
    }

    pred[city][set] = minCity;
    return mem[city][set] = minDist;
}

/**
 * Solves TSP using dynamic programming approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPDPSolver::solve() {
    if (tsp.empty()) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Number of subproblems
    spNum = 1u << (unsigned) tsp.getSize();
    // Full set mask
    FULL_SET = spNum - 1;
    // Memoization matrix of distances for combinations of sets and cities
    mem = new int *[tsp.getSize()];
    pred = new int *[tsp.getSize()];
    for (int i = 0; i < tsp.getSize(); ++i) {
        mem[i] = new int[spNum];
        fill(mem[i], mem[i] + spNum, -1);
        pred[i] = new int[spNum];
        fill(pred[i], pred[i] + spNum, -1);
    }

    // Resulting path
    Path resPath = Path(tsp.getSize() + 1);

    // Run Held Karp algorithm
    int result = HeldKarp(0, 1);
    resPath.setDistance(result);

    // Retrace path of the recursion using prev array
    int city = 0;
    unsigned int set = 1;
    int i = 0;
    while (true) {
        resPath.setPoint(i, city);
        city = pred[city][set];
        if (city == -1) break;
        set = set | (1u << (unsigned) city);
        ++i;
    }
    resPath.setPoint(++i, 0);

    // Free the memory
    for (int i = 0; i < tsp.getSize(); ++i) {
        delete[] mem[i];
        delete[] pred[i];
    }
    delete[] mem;
    delete[] pred;

    return resPath;
}
