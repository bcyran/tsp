//
// Created by bazyli on 25.10.18.
//

#ifndef C_TSPBNBSOLVER_H
#define C_TSPBNBSOLVER_H


#include "TSPSolver.h"

/**
 * Branch and bound solver for TSP.
 */
class TSPBnBSolver : public TSPSolver {

public:

    using TSPSolver::TSPSolver;

    Path solve() override;

};


#endif //C_TSPBNBSOLVER_H
