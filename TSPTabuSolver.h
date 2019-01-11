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
    int cadence = 18;

    /** Neighbourhood type: 0 - swap, 1 - insert, 2 - invert. */
    int neighbourhoodType = 2;

    /** Number of non-improving iterations before restarting with random path. */
    int resetThreshold = 45;

    /** Number of non-improving iterations before terminating search. **/
    int stopThreshold = 450;

    /** Maximum time of execution in ms. Overwrites iterations parameter and stop threshold. */
    int runTime = 0;

    /** Tabu list. */
    int **tabu = nullptr;

    void init();

    void clean();

    Path solveGreedy();

    void move(Path &path, int x, int y);

    Path minNeighbour(Path path);

    void updateTabu();

public:

    using TSPSolver::TSPSolver;

    explicit TSPTabuSolver(TSP tsp);

    virtual ~TSPTabuSolver();

    Path solve() override;

    void setTsp(TSP tsp) override;

    void setIterations(int iterations);

    void setCadence(int cadence);

    void setNeighbourhoodType(int type);

    void setResetThreshold(int resetThreshold);

    void setStopThreshold(int stopThreshold);

    void setRunTime(int runTime);

};


#endif //C_TSPLOCAL_H
