//
// Created by bazyli on 28.12.18.
//

#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <climits>
#include <chrono>
#include "TSPGenSolver.h"

/**
 * Returns random probability, a number between 0 and 1.
 *
 * @return Random probability.
 */
float TSPGenSolver::randomProb() {
    default_random_engine r(random_device{}());
    uniform_real_distribution<float> range(0.0, 1.0);

    return range(r);
}

/**
 * Sorts population in ascending order by path length.
 */
void TSPGenSolver::sortPopulation() {
    std::sort(population.begin(), population.end(), [](const auto &l, const auto &r) {
        return l.getDistance() < r.getDistance();
    });
}

/**
 * Fills population with random paths.
 */
void TSPGenSolver::initPopulation() {
    // Allocate memory for the population
    population.clear();
    population.reserve(static_cast<unsigned long>(populationSize));
    // Fill the population
    for (int i = 0; i < populationSize; i++) {
        Path path = Path(tsp.getSize() + 1);
        path.random();
        path.setDistance(tsp.pathDist(path));
        population.push_back(path);
    }
    sortPopulation();
}

/**
 * Fills mating pool with individuals chosen using Roulette Wheel Selection.
 */
void TSPGenSolver::selection() {
    // Clear the mating pool and allocate memory
    matingPool.clear();
    matingPool.reserve(static_cast<unsigned long>(populationSize));
    // Copy elite straight to the mating pool
    matingPool.insert(matingPool.end(), population.begin(), population.begin() + eliteSize);

    unsigned int sum = 0;
    unsigned int cumSum[populationSize];
    float prob[populationSize];

    // Calculate cumulative sum of distances for each individual and total sum
    for (int i = 0; i < populationSize; ++i) {
        sum += population[i].getDistance();
        cumSum[i] = sum;
    }
    // Calculate the cumulative probability that each individual will be NOT selected
    // Probability has to be "reversed" because it's a minimization problem: shorter = better
    for (int i = 0; i < populationSize; ++i) {
        prob[i] = static_cast<float>(cumSum[i]) / sum;
    }

    // Run for each empty slot in mating pool
    for (int i = eliteSize; i < populationSize; ++i) {
        // Spin te roulette
        float roulette = randomProb();
        for (int j = 0; j < populationSize; ++j) {
            if (roulette <= prob[j]) {
                matingPool.push_back(population[j]);
                break;
            }
        }
    }
}

/**
 * Randomly draws to locations in path excluding first and last.
 *
 * @return Pair consisting of indexes in path.
 */
pair<int, int> TSPGenSolver::randSubpath() {
    // Child path
    Path child = Path(tsp.getSize() + 1);
    child.setPoint(0, 0);
    child.setPoint(tsp.getSize(), 0);

    // Generate two random positions in paths
    default_random_engine r(random_device{}());
    // Positions can't be the first or last city of path
    uniform_int_distribution<int> range(1, tsp.getSize() - 1);
    int x = range(r);
    int y;
    // Positions can't be equal
    do {
        y = range(r);
    } while (x == y);

    return {min(x, y), max(x, y)};
}

/**
 * Performs order 1 crossover on two parents paths to create a child path.
 *
 * @param parent1 First parent path.
 * @param parent2 Second parent path.
 * @return Child path.
 */
Path TSPGenSolver::ox(Path parent1, Path parent2) {
    // Child path
    Path child = Path(tsp.getSize() + 1);
    child.setPoint(0, 0);
    child.setPoint(tsp.getSize(), 0);

    // Random subpath
    pair<int, int> sub = randSubpath();
    int startPos = sub.first;
    int endPos = sub.second;

    // Copy part between start and end positions from first parent 1 to child
    for (int i = startPos; i <= endPos; ++i) {
        child.setPoint(i, parent1.getPoint(i));
    }
    // Fill in empty cities with cities from parent 2 in order
    int parentPos = 1;
    int childPos = 1;
    while (parentPos < tsp.getSize()) {
        // If we are inside range copied from parent 1 jump to the and of it
        if (childPos >= startPos && childPos <= endPos) {
            childPos = endPos + 1;
            continue;
        }

        // Get city from current parent position
        int parentCity = parent2.getPoint(parentPos);
        if (child.inPath(parentCity, tsp.getSize())) {
            // If this city is already in child path increment parent position and continue
            ++parentPos;
            continue;
        } else {
            // If city is not in child insert it in and increment both counters
            child.setPoint(childPos, parentCity);
            ++childPos;
            ++parentPos;
        }
    }

    child.setDistance(tsp.pathDist(child));
    return child;
}

/**
 * Performs PMX crossover on two parents paths to create a child path.
 *
 * @param parent1 First parent path.
 * @param parent2 Second parent path.
 * @return Child path.
 */
Path TSPGenSolver::pmx(Path parent1, Path parent2) {
    // Child path
    Path child = Path(tsp.getSize() + 1);
    child.setPoint(0, 0);
    child.setPoint(tsp.getSize(), 0);

    // Random subpath
    pair<int, int> sub = randSubpath();
    int startPos = sub.first;
    int endPos = sub.second;

    // Create mapping array and it with -1
    auto mapping = new int[tsp.getSize() + 1];
    fill(mapping, mapping + tsp.getSize() + 1, -1);

    // Copy part between start and end positions from parent 1 to child
    // and create mapping
    for (int i = startPos; i <= endPos; ++i) {
        child.setPoint(i, parent1.getPoint(i));
        mapping[parent1.getPoint(i)] = parent2.getPoint(i);
    }

    // Copy other cities from parent2 to child using mapping if necessary
    for (int i = 1; i < tsp.getSize(); ++i) {
        // If we are inside range copied from parent 1 jump to the and of it
        if (i >= startPos && i <= endPos) {
            i = endPos;
            continue;
        }

        // Get city at current index from parent2
        int value = parent2.getPoint(i);

        // Trace mapping if exists
        while (mapping[value] != -1) {
            value = mapping[value];
        }

        // Set point in child path
        child.setPoint(i, value);
    }

    delete[] mapping;

    child.setDistance(tsp.pathDist(child));
    return child;
}

/**
 * Performs non-wrapping order crossover on two parents paths to create a child path.
 *
 * @param parent1 First parent path.
 * @param parent2 Second parent path.
 * @return Child path.
 */
Path TSPGenSolver::nwox(Path parent1, Path parent2) {
    // Child path
    Path child = Path(tsp.getSize() + 1);
    child.setPoint(0, 0);
    child.setPoint(tsp.getSize(), 0);

    // Random subpath
    pair<int, int> sub = randSubpath();
    int startPos = sub.first;
    int endPos = sub.second;

    // Already copied cities
    auto used = new bool[tsp.getSize() + 1];
    fill(used, used + tsp.getSize(), false);
    // Copy part between start and end positions from parent 1 to child
    for (int i = startPos; i <= endPos; ++i) {
        child.setPoint(i, parent1.getPoint(i));
        used[parent1.getPoint(i)] = true;
    }

    // Copy cities from parent2 if those aren't already in child (preserve original positions)
    for (int i = 1; i < tsp.getSize(); ++i) {
        // If we are inside range copied from parent 1 jump to the and of it
        if (i >= startPos && i <= endPos) {
            i = endPos;
            continue;
        }

        // Skip city if it's already used
        if (used[parent2.getPoint(i)]) continue;

        // Set point in child path
        child.setPoint(i, parent2.getPoint(i));
        used[parent2.getPoint(i)] = true;
    }

    // "Slide" the cities outside of the copied range to the edges
    // From the left
    for (int i = 1; i < startPos; ++i) {
        // If current position is empty
        if (child.getPoint(i) != -1) continue;

        // Search for the next not empty position
        int j = i;
        while (j < startPos - 1 && child.getPoint(j) == -1) { ++j; }

        // And set it's value as current position
        child.setPoint(i, child.getPoint(j));
        child.setPoint(j, -1);
    }
    // And from the right
    for (int i = tsp.getSize() - 1; i > endPos; --i) {
        if (child.getPoint(i) != -1) continue;

        int j = i;
        while (j > endPos + 1 && child.getPoint(j) == -1) { --j; }

        child.setPoint(i, child.getPoint(j));
        child.setPoint(j, -1);
    }

    // Current index of path2 subpath
    int subi = startPos;
    // Fill empty spaces with cities from parent2 subpath
    for (int i = 1; i < tsp.getSize(); ++i) {
        // If we are inside range copied from parent 1 jump to the and of it
        if (i >= startPos && i <= endPos) {
            i = endPos;
            continue;
        }

        // If current position in child isn't empty skip iterations
        if (child.getPoint(i) != -1) continue;

        // If current city in parent2 subpath is used advance to the next ione
        while (used[parent2.getPoint(subi)]) ++subi;

        // Set point in child path
        child.setPoint(i, parent2.getPoint(subi));
        ++subi;
    }

    delete[] used;

    child.setDistance(tsp.pathDist(child));
    return child;
}

/**
 * Performs chosen crossover on given paths.
 *
 * @param parent1 First parent path.
 * @param parent2 Second parent path.
 * @return Child path.
 */
Path TSPGenSolver::crossover(Path parent1, Path parent2) {
    switch (crossoverType) {
        case Crossover::OX:
            return ox(parent1, parent2);
        case Crossover::PMX:
            return pmx(parent1, parent2);
        case Crossover::NWOX:
            return nwox(parent1, parent2);
    }
}

/**
 * Creates new population by breeding the mating pool.
 * Breeding consists of crossing over each individual with the next one in the mating pool.
 */
void TSPGenSolver::breed() {
    // Clear the current population and allocate memory
    population.clear();
    population.reserve(static_cast<unsigned long>(populationSize));
    // Retain the elite without crossing over
    population.insert(population.end(), matingPool.begin(), matingPool.begin() + eliteSize);
    // Crossover every parent from the mating pool with the next one
    for (int i = eliteSize; i < populationSize - 1; ++i) {
        population.push_back(crossover(matingPool[i], matingPool[i + 1]));
    }
    // Wrap around to the start, crossover last parent with first parent
    population.push_back(crossover(matingPool[populationSize - 1], matingPool[0]));
}

/**
 * Mutates given individual by swapping cities with probability defined by mutationRate.
 *
 * @param path Individual to mutate.
 */
void TSPGenSolver::mutation(Path &path) {
    default_random_engine r(random_device{}());
    uniform_int_distribution<int> range(1, tsp.getSize() - 1);

    // Randomly choose to points in path
    int x = range(r);
    int y;
    do {
        y = range(r);
    } while (x == y);

    // Invert cities between these points
    path.invert(x, y);
    path.setDistance(tsp.pathDist(path));
}

/**
 * Mutates the population by applying mutate method to each individual.
 */
void TSPGenSolver::mutate() {
    for (int i = eliteSize; i < populationSize - 1; ++i) {
        if (randomProb() >= mutationRate) continue;
        mutation(population[i]);
    }
}

/**
 * Solves TSP using Genetic Algorithm.
 *
 * @return Best found path.
 */
Path TSPGenSolver::solve() {
    if (tsp.empty()) {
        throw runtime_error("Cannot solve empty problem.");
    }

    // Initialize population and minimum path
    initPopulation();
    Path minPath = population.front();

    // Loop through specified number of generations or run time
    int evolved = 0;
    auto startTime = chrono::high_resolution_clock::now();
    while (true) {
        // Perform selection, breeding and mutation
        selection();
        breed();
        mutate();

        // Sort the population
        sortPopulation();

        // If best path from this generation is better than min path set it as min
        if (population.front().getDistance() < minPath.getDistance()) {
            minPath = population.front();
        }

        // Stop conditions
        if (runTime) {
            // Calculate execution time and break if it exceeds maximum time
            auto currentTime = chrono::high_resolution_clock::now();
            int elapsedTime = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(
                    currentTime - startTime).count());

            if (elapsedTime > runTime) break;
        } else {
            // Increment generations counter and break if it exceeds maximum number of generations
            ++evolved;
            if (evolved > generations) break;
        }
    }

    return minPath;
}

/**
 * Sets size of the single population.
 *
 * @param populationSize Size of the population.
 */
void TSPGenSolver::setPopulationSize(int populationSize) {
    TSPGenSolver::populationSize = populationSize;
}

/**
 * Sets number of individuals carried over to the next generation.
 *
 * @param eliteSize Size of the elite.
 */
void TSPGenSolver::setEliteSize(int eliteSize) {
    TSPGenSolver::eliteSize = eliteSize;
}

/**
 * Sets rate at which mutations occur.
 *
 * @param mutationRate Mutation rate.
 */
void TSPGenSolver::setMutationRate(float mutationRate) {
    TSPGenSolver::mutationRate = mutationRate;
}

/**
 * Set number of generations to evolve.
 *
 * @param generations Number of generations.
 */
void TSPGenSolver::setGenerations(int generations) {
    TSPGenSolver::generations = generations;
}

/**
 * Sets maximum time of execution in ms.
 *
 * @param runTime Time in ms.
 */
void TSPGenSolver::setRunTime(int runTime) {
    TSPGenSolver::runTime = runTime;
}

/**
 * Sets the path crossover type.
 *
 * @param crossoverType
 */
void TSPGenSolver::setCrossoverType(Crossover crossoverType) {
    TSPGenSolver::crossoverType = crossoverType;
}
