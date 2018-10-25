//
// Created by bazyli on 25.10.18.
//

#ifndef C_TSPSOLVER_H
#define C_TSPSOLVER_H


#include "TSP.h"
#include "Path.h"

/**
 * Abstract solver of TSP.
 * Stores TSP instance, provides access to distances between cities, path distances, problem size.
 */
class TSPSolver {

protected:

    /** Instance of TSP problem. */
    TSP tsp;

public:

    TSPSolver();

    explicit TSPSolver(TSP tsp);

    virtual Path solve() = 0;

    void setTsp(TSP tsp);

    void random(int size);
};


#endif //C_TSPSOLVER_H
