//
// Created by bazyli on 25.10.18.
//

#include <stdexcept>
#include <climits>
#include <stack>
#include <tuple>
#include "TSPBnBSolver.h"

/**
 * Node used in branch and bound consists of city number, distance and level.
 */
typedef tuple<int, int, int> node;

/**
 * Solves TSP using branch and bound approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPBnBSolver::solve() {
    if (tsp.empty()) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Shortest path
    Path minPath;
    // Minimal distance
    int minDist = INT_MAX;
    // Nodes queue
    stack<node> stack;
    // Current path for each node, additional index for return to start
    Path curPath = Path(tsp.getSize() + 1);
    curPath.setPoint(tsp.getSize(), 0);

    // Add starting city to stack
    stack.push(make_tuple(0, 0, 0));

    // While stack is not empty
    while (!stack.empty()) {
        // Get last added city from the stack
        node curNode = stack.top();
        int city = get<0>(curNode);
        int dist = get<1>(curNode);
        int level = get<2>(curNode);
        int nextLevel = level + 1; // Level of all children of current node
        stack.pop();
        curPath.setPoint(level, city); // Update current path with current node

        // If it's last level of the tree
        if (level == tsp.getSize() - 1) {
            // Add return to 0 to the distance and check if it's lower than current min distance
            if ((dist + tsp.dist(city, 0)) < minDist) {
                minPath = curPath;
                minDist = dist + tsp.dist(city, 0);
            } else {
                continue;
            }
        }

        // Iterate through all nodes
        for (int i = 0; i < tsp.getSize(); ++i) {

            // Skip this city itself, its predecessors and starting city
            if (i == city || curPath.inPath(i, nextLevel) || i == 0) continue;

            // Distance of this node, skip it if it's bigger than minimum full path
            int nextDist = dist + tsp.dist(city, i);
            if (nextDist >= minDist) continue;

            // If it's valid insert into stack
            node nextNode = make_tuple(i, nextDist, nextLevel);
            stack.push(nextNode);
        }
    }
    minPath.setDistance(tsp.pathDist(minPath));

    return minPath;
}
