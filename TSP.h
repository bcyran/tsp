//
// Created by bazyli on 11.10.18.
//

#ifndef C_TSP_H
#define C_TSP_H

#include <string>
#include "Path.h"

using namespace std;

/**
 * Represents Travelling Salesman Problem instance.
 */
class TSP {

    /** Size of the problem (number of the cities). */
    int size = 0;

    /** Matrix of distances between each city. */
    int **distance = nullptr;

    void initialize(int size);

public:

    TSP();

    virtual ~TSP();

    TSP(const TSP &src);

    TSP &operator=(const TSP &src);

    void load(string file);

    void random(int size);

    int dist(int i, int j);

    int pathDist(Path path);

    int getSize() const;

    bool empty();

    string toString();

};


#endif //C_TSP_H
