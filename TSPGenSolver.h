//
// Created by bazyli on 28.12.18.
//

#ifndef TSP_TSPGENSOLVER_H
#define TSP_TSPGENSOLVER_H

#include <vector>
#include "TSPSolver.h"
#include "Crossover.h"

/**
 * Genetic Algorithm solver for TSP.
 */
class TSPGenSolver : public TSPSolver {

    /** Size of the one population. */
    int populationSize = 100;

    /** Number of best individuals carried over to the next generation. */
    int eliteSize = 40;

    /** Rate at which mutations occur. */
    float mutationRate = 0.04;

    /** Number of generations to evolve. */
    int generations = 2000;

    /** Maximum time of execution in ms. Overwrites generations parameter. */
    int runTime = 0;

    /** Crossover type: OX, PMX or NWOX */
    Crossover crossoverType = Crossover::NWOX;

    /** Current population. */
    vector<Path> population;

    /** Current mating pool. */
    vector<Path> matingPool;

    float randomProb();

    void sortPopulation();

    void initPopulation();

    void selection();

    pair<int, int> randSubpath();

    Path ox(Path parent1, Path parent2);

    Path pmx(Path parent1, Path parent2);

    Path nwox(Path parent1, Path parent2);

    Path crossover(Path parent1, Path parent2);

    void breed();

    void mutation(Path &path);

    void mutate();

public:

    using TSPSolver::TSPSolver;

    Path solve() override;

    void setPopulationSize(int populationSize);

    void setEliteSize(int eliteSize);

    void setMutationRate(float mutationRate);

    void setGenerations(int generations);

    void setRunTime(int runTime);

    void setCrossoverType(Crossover crossoverType);

};


#endif //TSP_TSPGENSOLVER_H
