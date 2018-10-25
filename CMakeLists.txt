cmake_minimum_required(VERSION 3.12)
project(C__)

set(CMAKE_CXX_STANDARD 14)

add_executable(tsp main.cpp TSP.cpp TSP.h Path.cpp Path.h TimeTester.cpp TimeTester.h TSPBFSolver.cpp TSPBFSolver.h TSPSolver.cpp TSPSolver.h TSPBnBSolver.cpp TSPBnBSolver.h TSPDPSolver.cpp TSPDPSolver.h TSPTabuSolver.cpp TSPTabuSolver.h)