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

protected:

    /** Size of the problem (number of the cities). */
    int size = 0;

    /** Matrix of distances between each city. */
    int **distance = nullptr;

    void initialize(int size);

    int dist(Path path);

public:

    TSP();

    virtual ~TSP();

    void load(string file);

    void random(int size);

    string toString();

};


#endif //C_TSP_H
