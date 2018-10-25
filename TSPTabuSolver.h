//
// Created by bazyli on 24.10.18.
//

#ifndef C_TSPLOCAL_H
#define C_TSPLOCAL_H


#include "TSP.h"
#include "TSPSolver.h"

/**
 * Tabu search solver for TSP.
 */
class TSPTabuSolver : public TSPSolver {

    /** Number of iterations. */
    int iterations = 1000;

    /** Tabu cadence. */
    int cadence = 5;

    Path solveGreedy();

    Path minNeighbour(Path path, int **tabu, int cadence);

    void updateTabu(int **tabu);

public:

    using TSPSolver::TSPSolver;

    Path solve() override;

    void setIterations(int iterations);

    void setCadence(int cadence);

};


#endif //C_TSPLOCAL_H
