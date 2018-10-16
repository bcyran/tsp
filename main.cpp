#include <iostream>
#include "TSP.h"
#include "TSPExact.h"

int main() {
    TSPExact tsp;
    tsp.load("../dane/tsp_6_2.txt");
    cout << tsp.toString();
    Path solution = tsp.solveDP();
    cout << solution.toString();


}