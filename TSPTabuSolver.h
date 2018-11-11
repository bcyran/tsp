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
    int cadence = 100;

    /** Neighbourhood type: 0 - swap, 1 - insert, 2 - invert. */
    int neighbourhoodType = 0;

    /** Number of non-improving solutions before restarting with random path. */
    int incorrectThreshold = 50;

    /** Tabu list. */
    int **tabu = nullptr;

    void init();

    void clean();

    Path solveGreedy();

    Path randomPath();

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

    void setIncorrectThreshold(int threshold);

};


#endif //C_TSPLOCAL_H
