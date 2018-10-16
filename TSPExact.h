//
// Created by bazyli on 16.10.18.
//

#ifndef C_TSPEXACT_H
#define C_TSPEXACT_H

#include "TSP.h"

using namespace std;

/**
 * Provides methods for solving Travelling Salesman Problem using exact algorithms:
 * brute-force, branch and bound, dynamic programming.
 */
class TSPExact : public TSP {

    unsigned long int factorial(int x);

public:

    Path solveBF();

    Path solveBnB();

    Path solveDP();

    int HeldKarp(int start, int set, int **mem, int fullSet);

};


#endif //C_TSPEXACT_H