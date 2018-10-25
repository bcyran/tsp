//
// Created by bazyli on 25.10.18.
//

#ifndef C_TSPBFSOLVER_H
#define C_TSPBFSOLVER_H


#include "TSPSolver.h"

/**
 * Brute-force solver for TSP.
 */
class TSPBFSolver : public TSPSolver {

public:

    using TSPSolver::TSPSolver;

    Path solve() override;

};


#endif //C_TSPBFSOLVER_H
