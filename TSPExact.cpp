//
// Created by bazyli on 16.10.18.
//

#include <climits>
#include <algorithm>
#include <tuple>
#include "TSPExact.h"
#include "List.h"

/**
 * Node used in branch and bound consists of city number, distance and level.
 */
typedef tuple<int, int, int> node;

/**
 * Helper function calculating factorial value.
 *
 * Unsigned long int should be enough to completely fill most computers' RAM with permutations.
 *
 * @param x Number to calculate the factorial of.
 * @return Factorial of given number.
 */
unsigned long int TSPExact::factorial(int x) {
    unsigned long int result = 1;

    for (int i = 1; i <= x; ++i) {
        result *= i;
    }

    return result;
}

/**
 * Solves TSP using brute-force approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPExact::solveBF() {
    // Array with numbers of cities
    // Additional index for return to the starting point
    int *cities = new int[size + 1];
    for (int i = 0; i < size; ++i) {
        cities[i] = i;
    }
    cities[size] = cities[0];

    // Array of permutations
    unsigned long int perNum = factorial(size - 1);
    Path *paths = new Path[perNum];

    // Generate all permutations
    for (unsigned long int i = 0; i < perNum; ++i) {
        paths[i] = Path(size + 1, cities);
        next_permutation(cities, cities + size);
    }

    // Find shortest path
    Path minPath = paths[0];
    int minDist = dist(minPath);
    int tmpDist = 0;
    for (unsigned long int i = 1; i < perNum; ++i) {
        tmpDist = dist(paths[i]);
        if (tmpDist < minDist) {
            minPath = paths[i];
            minDist = tmpDist;
        }
    }
    minPath.setDistance(dist(minPath));

    return minPath;
}

/**
 * Solves TSP using branch and bound approach.
 *
 * @return The shortest possible cycle.
 */
Path TSPExact::solveBnB() {
    // Shortest path
    Path minPath;
    // Minimal distance
    int minDist = INT_MAX;
    // Nodes queue
    List<node> queue;
    // Current path for each node, additional index for return to start
    Path curPath = Path(size + 1);
    curPath.setPoint(size, 0);

    // Enqueue starting city
    queue.pushBack(make_tuple(0, 0, 0));

    // While queue is not empty
    while (queue.getLength()) {
        // Get last added city from the queue (LIFO queue)
        node curNode = queue.get(queue.getLength() - 1);
        int city = get<0>(curNode);
        int dist = get<1>(curNode);
        int level = get<2>(curNode);
        int nextLevel = level + 1; // Level of all children of current node
        queue.popBack();
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

            // If it's valid insert into queue
            node nextNode = make_tuple(i, nextDist, nextLevel);
            queue.pushBack(nextNode);
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
    // Resulting path
    Path resPath = Path(size + 1);
    // Number of subproblems
    int spNum = (1 << size);
    // Full set mask
    int fullSet = spNum - 1;
    // Memoization matrix of distances for combinations of sets and cities
    int **mem = new int *[size];
    int **prev = new int *[size];
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
    int set = 1;
    int i = 0;
    while (true) {
        resPath.setPoint(i, city);
        city = prev[city][set];
        if (city == -1) break;
        set = set | (1 << city);
        ++i;
    }
    resPath.setPoint(++i, 0);

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
int TSPExact::HeldKarp(int city, int set, int **mem, int **prev, int fullSet) {
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
        int mask = 1 << i;

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
