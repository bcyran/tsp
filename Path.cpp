//
// Created by bazyli on 11.10.18.
//

#include <cstring>
#include <sstream>
#include "Path.h"

/**
 * Creates empty path object.
 */
Path::Path() = default;

/**
  * Creates path object of given length filled with -1.
  *
  * @param length Length of the path.
  */
Path::Path(int length)  : length(length), path(new int[length]) {
    fill(path, path + length, -1);
}

/**
 * Creates Path object containing given path and its length.
 *
 * @param len Length of the path.
 * @param path Path as array of cities.
 */
Path::Path(int length, int *path) : length(length), path(new int[length]) {
    memcpy(this->path, path, sizeof(path[0]) * length);
}

/**
 * Copy constructor.
 *
 * @param src Source Path object.
 */
Path::Path(const Path &src) : length(src.length), distance(src.distance), path(new int[src.length]) {
    memcpy(this->path, src.path, sizeof(src.path[0]) * length);
}

/**
 * Assignment operator.
 *
 * @param obj Source path object.
 * @return Assgined object.
 */
Path &Path::operator=(const Path &src) {
    if (this != &src) {
        delete[] path;
        length = src.length;
        distance = src.distance;
        path = new int[length];
        memcpy(path, src.path, sizeof(src.path[0]) * length);
    }

    return *this;
}

/**
 * Frees up memory by deleting path array.
 */
Path::~Path() {
    delete[] path;
}

/**
 * Checks whether given city is in n first elements of the path.
 *
 * @param city City to check.
 * @param limit Number of path elements to search, counted from the start.
 * @return True if city is in path.
 */
bool Path::inPath(int city, int limit) {
    for (int i = 0; i < limit; ++i) {
        if (path[i] == city) {
            return true;
        }
    }

    return false;
}

/**
 * Use Knuth's "Algorithm L" to generate next lexicographical permutation of cities.
 * Works just like next_permutation().
 *
 * @param start Start index of permutation.
 * @param end End index of permutation.
 * @return True if permutation was successful.
 */
bool Path::permute(int start, int end) {
    // Nothing to do for empty or single city path
    if (end - start <= 1) return false;

    // Find last i such that path[i] <= path[i+1].
    // If it doesn't exist path is at final permutation, return false.
    int i = end - 1;
    while (path[i] > path[i + 1]) {
        if (--i == start - 1) return false;
    }

    // Find last j such that path[i] <= path[j]
    int j = end;
    while (path[i] > path[j]) {
        --j;
    }

    // Swap out i and j
    std::swap(path[i], path[j]);

    // Reverse cities from j+1 to length-1
    int lo = i + 1;
    int hi = end;
    while (lo < hi) {
        std::swap(path[lo], path[hi]);
        ++lo;
        --hi;
    }

    return true;
}

/**
 * Swaps cities with given indexes.
 *
 * @param x Index of first city to swap.
 * @param y Index of second city to swap.
 */
void Path::swap(int x, int y) {
    std::swap(path[x], path[y]);
}

/**
 * Length getter.
 *
 * @return The length.
 */
int Path::getLength() const {
    return length;
}

/**
 * Returns city number of given step in a path.
 *
 * @param index Index of city (step in a path).
 * @return City number.
 */
int Path::getPoint(int index) {
    return path[index];
}

/**
 * Sets given point to the given city.
 *
 * @param index Index of point to set.
 * @param city City to set.
 * @return
 */
void Path::setPoint(int index, int city) {
    path[index] = city;
}

/**
 * Path array getter.
 *
 * @return Path array pointer.
 */
int *Path::getPath() const {
    return path;
}

/**
 * Distance getter.
 *
 * @return Distance.
 */
int Path::getDistance() const {
    return distance;
}

/**
 * Distance setter.
 *
 * @param distance Distance.
 */
void Path::setDistance(int distance) {
    Path::distance = distance;
}

/**
 * Creates string representation of the Path.
 *
 * @return String representation.
 */
string Path::toString() {
    ostringstream output;

    for (int i = 0; i < length; ++i) {
        output << path[i] << (i < length - 1 ? " - " : "");
    }
    output << " (" << distance << ")";

    return output.str();
}
