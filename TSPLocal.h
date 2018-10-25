//
// Created by bazyli on 24.10.18.
//

#ifndef C_TSPLOCAL_H
#define C_TSPLOCAL_H


#include "TSP.h"

/**
 * Provides methods for solving Travelling Salesman Problem using local search algorithms.
 */
class TSPLocal : public TSP {

    Path solveGreedy();

    Path minNeighbour(Path path, int **tabu, int cadence);

    void updateTabu(int **tabu);

public:

    Path solveTabu();

};


#endif //C_TSPLOCAL_H
