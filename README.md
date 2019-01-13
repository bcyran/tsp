# TSP
This repo contains university project which consists of implementations of various algorithms for solving Travelling Salesman Problem. There's also very simple text interface allowing to read TSP instance in matrix form from txt file and solve it using any implemented algorithm.

## Algorithms
* Exact algorithms
  * Brute force
  * Branch and bound
  * Dynamic Programming (Held-Karp algorithm)
* Metaheuristics
  * Tabu search
  * Simulated annealing
  * Genetic algorithm

## Tuning
Effectiveness of metaheuristics is hugely dependent on their parameters. Metaheuristics implemented in this project all have default parameters which were optimized during tests on TSP instances not bigger than 120 cities. However if you would like to have better control over those algorithms you can tune the following parameters:

### Tabu search
* Number of iterations
* Tabu cadence
* Neighbourhood type: swap, insert or invert
* Reset threshold - number of non-improving iterations causing abandoning of the current path and starting from random one
* Stop threshold - number of non-improving iterations until the solver stops
* Run time - time of executions in ms. Overwrites iterations number and stop threshold.

### Simulated annealing
* Initial temperature
* End temperature
* Cooling rate
* Neighbourhood type: swap, insert or invert
* Number of iterations for each temperature
* Run time - time of executions in ms. Overwrites iterations number.

### Genetic algorithm
* Population size
* Elite size
* Mutation rate
* Number of generations
* Crossover type: OX, PMX or NWOX
* Run time - time of executions in ms. Overwrites generations number.
