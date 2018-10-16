cmake_minimum_required(VERSION 3.12)
project(C__)

set(CMAKE_CXX_STANDARD 14)

add_executable(pea-projekt1 main.cpp TSP.cpp TSP.h Path.cpp Path.h List.h List.tcc ListElement.h ListElement.tcc TSPExact.cpp TSPExact.h)