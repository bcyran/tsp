//
// Created by bazyli on 25.10.18.
//

#include <stdexcept>
#include <iostream>
#include "TSPBFSolver.h"

/**
 * Solves TSP using brute-force approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPBFSolver::solve() {
    if (tsp.empty()) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Working path that will be permuted
    Path path = Path(tsp.getSize() + 1);
    for (int i = 0; i < tsp.getSize(); ++i) {
        path.setPoint(i, i);
    }
    path.setPoint(tsp.getSize(), 0);
    path.setDistance(tsp.pathDist(path));

    // Set min path as current working path
    Path minPath = path;

    // Generate all permutations
    while (path.permute(1, tsp.getSize() - 1)) {
        // If generated path is shorter than current minimum set it as minimum
        path.setDistance(tsp.pathDist(path));
        if (path.getDistance() < minPath.getDistance()) {
            minPath = path;
        }
    }

    return minPath;
}
