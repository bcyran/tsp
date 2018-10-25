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

    int HeldKarp(int start, unsigned int set, int **mem, int **prev, unsigned int fullSet);

public:

    Path solveBF();

    Path solveBnB();

    Path solveDP();

};


#endif //C_TSPEXACT_H
