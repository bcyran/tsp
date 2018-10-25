//
// Created by bazyli on 16.10.18.
//

#include <climits>
#include <tuple>
#include <stack>
#include "TSPExact.h"

/**
 * Node used in branch and bound consists of city number, distance and level.
 */
typedef tuple<int, int, int> node;

/**
 * Solves TSP using brute-force approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPExact::solveBF() {
    if (size <= 0 || distance == nullptr) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Working path that will be permuted
    Path path = Path(size + 1);
    for (int i = 0; i < size; ++i) {
        path.setPoint(i, i);
    }
    path.setPoint(size, 0);
    path.setDistance(dist(path));

    // Set min path as current working path
    Path minPath = path;

    // Generate all permutations
    while (path.permute(1, size - 1)) {
        // If generated path is shorter than current minimum set it as minimum
        path.setDistance(dist(path));
        if (path.getDistance() < minPath.getDistance()) {
            minPath = path;
        }
    }

    return minPath;
}

/**
 * Solves TSP using branch and bound approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPExact::solveBnB() {
    if (size <= 0 || distance == nullptr) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Shortest path
    Path minPath;
    // Minimal distance
    int minDist = INT_MAX;
    // Nodes queue
    stack<node> stack;
    // Current path for each node, additional index for return to start
    Path curPath = Path(size + 1);
    curPath.setPoint(size, 0);

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
        if (level == size - 1) {
            // Add return to 0 to the distance and check if it's lower than current min distance
            if ((dist + distance[city][0]) < minDist) {
                minPath = curPath;
                minDist = dist + distance[city][0];
            } else {
                continue;
            }
        }

        // Iterate through all nodes
        for (int i = 0; i < size; ++i) {

            // Skip this city itself, its predecessors and starting city
            if (i == city || curPath.inPath(i, nextLevel) || i == 0) continue;

            // Distance of this node, skip it if it's bigger than minimum full path
            int nextDist = dist + distance[city][i];
            if (nextDist >= minDist) continue;

            // If it's valid insert into stack
            node nextNode = make_tuple(i, nextDist, nextLevel);
            stack.push(nextNode);
        }
    }
    minPath.setDistance(dist(minPath));

    return minPath;
}

/**
 * Solves TSP using dynamic programming approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPExact::solveDP() {
    if (size <= 0 || distance == nullptr) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Resulting path
    Path resPath = Path(size + 1);
    // Number of subproblems
    unsigned int spNum = 1u << (unsigned) size;
    // Full set mask
    unsigned int fullSet = spNum - 1;
    // Memoization matrix of distances for combinations of sets and cities
    auto mem = new int *[size];
    auto prev = new int *[size];
    for (int i = 0; i < size; ++i) {
        mem[i] = new int[spNum];
        fill(mem[i], mem[i] + spNum, -1);
        prev[i] = new int[spNum];
        fill(prev[i], prev[i] + spNum, -1);
    }

    // Run Held Karp algorithm
    int result = HeldKarp(0, 1, mem, prev, fullSet);
    resPath.setDistance(result);

    // Retrace path of the recursion using prev array
    int city = 0;
    unsigned int set = 1;
    int i = 0;
    while (true) {
        resPath.setPoint(i, city);
        city = prev[city][set];
        if (city == -1) break;
        set = set | (1u << (unsigned) city);
        ++i;
    }
    resPath.setPoint(++i, 0);

    // Free the memory
    for (int i = 0; i < size; ++i) {
        delete[] mem[i];
        delete[] prev[i];
    }
    delete[] mem;
    delete[] prev;

    return resPath;
}

/**
 * Implements recursive Held-Karp algorithm.
 *
 * @param city Current city.
 * @param set Set of visited cities.
 * @param mem Pointer to memoization array.
 * @param fullSet Full set.
 * @return Min path for given input.
 */
int TSPExact::HeldKarp(int city, unsigned int set, int **mem, int **prev, unsigned int fullSet) {
    // If all nodes are visited
    if (set == fullSet) {
        return distance[city][0];
    }

    // If this combination of city and set is already memorized.
    if (mem[city][set] != -1) {
        return mem[city][set];
    }

    // Current minimum distance and city
    int minDist = INT_MAX;
    int minCity = -1;

    // Iterate through all unvisited cities
    for (int i = 0; i < size; ++i) {
        unsigned int mask = 1u << (unsigned) i;

        // If i city wasn't visited
        if (!(set & mask)) {
            // Enter next recursion level with masked current node
            int dist = distance[city][i] + HeldKarp(i, set | mask, mem, prev, fullSet);
            // If new distance is shorter than current min set it as min
            if (dist < minDist) {
                minDist = dist;
                minCity = i;
            }
        }
    }

    prev[city][set] = minCity;
    return mem[city][set] = minDist;
}
