//
// Created by bazyli on 11.10.18.
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <random>
#include "TSP.h"
#include "Path.h"

/**
 * Creates empty TSP instance.
 */
TSP::TSP() = default;

/**
 * Frees up memory by deleting distance matrix.
 */
TSP::~TSP() {
    for (int i = 0; i < size; ++i) {
        delete[] distance[i];
    }
    delete[] distance;
}

/**
 * Initializes empty distance matrix of given size.
 *
 * @param size Number of cities.
 */
void TSP::initialize(int size) {
    this->size = size;
    distance = new int *[size];

    for (int i = 0; i < size; ++i) {
        distance[i] = new int[size];

        for (int j = 0; j < size; ++j) {
            distance[i][j] = 0;
        }
    }
}

/**
 * Calculates distance of given path.
 *
 * @param path Path object.
 * @return Distance of path.
 */
int TSP::dist(Path path) {
    int dist = 0;

    for (int i = 0; i < path.getLength() - 1; ++i) {
        dist += distance[path.getPoint(i)][path.getPoint(i + 1)];
    }

    return dist;
}

/**
 * Loads problem from file.
 *
 * @param file Name of file to load data from.
 */
void TSP::load(string file) {
    ifstream input;
    input.open(file);

    if (!input.is_open()) {
        throw invalid_argument("Requested file does not exist.");
    }

    input >> size;
    initialize(size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input >> distance[i][j];
        }
    }

    input.close();
}

/**
 * Generates random problem instance of given size.
 *
 * @param size Size of the problem.
 */
void TSP::random(int size) {
    initialize(size);

    default_random_engine r(random_device{}());
    uniform_int_distribution<int> distRange(0, 100);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                continue;
            }
            distance[i][j] = distRange(r);
        }
    }
}

/**
 * Creates string representation of the TSP.
 *
 * @return String representation.
 */
string TSP::toString() {
    ostringstream output;
    int padding = 3;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            output << setfill(' ') << setw(padding) << distance[i][j];
        }
        output << endl;
    }

    return output.str();
}
