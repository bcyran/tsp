cmake_minimum_required(VERSION 3.12)
project(tsp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_STANDARD 14)

add_executable(tsp main.cpp TSP.cpp TSP.h Path.cpp Path.h TimeTester.cpp TimeTester.h TSPBFSolver.cpp TSPBFSolver.h TSPSolver.cpp TSPSolver.h TSPBnBSolver.cpp TSPBnBSolver.h TSPDPSolver.cpp TSPDPSolver.h TSPTabuSolver.cpp TSPTabuSolver.h TSPSimAnnealingSolver.cpp TSPSimAnnealingSolver.h TSPGenSolver.cpp TSPGenSolver.h)