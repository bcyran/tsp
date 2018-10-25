#include <iostream>
#include <functional>
#include <iomanip>
#include <fstream>
#include "TimeTester.h"
#include "TSPTabuSolver.h"
#include "TSPSolver.h"
#include "TSPBFSolver.h"
#include "TSPBnBSolver.h"
#include "TSPDPSolver.h"

using namespace std;

/**
 * Measures time of execution of each method, specified number of times for all problem sizes up to given maximum.
 *
 * @param file Output file path (CSV format).
 * @param testNum Number of tests of each method for one size.
 * @param maxSize Maximum size of the problem to measure.
 */
void measure(const string &file, int testNum, int maxSize) {
    // limits of size for each method to avoid too long measurements
    int BFMax = 12;
    int BnBMax = 18;
    int DPMax = 23;

    TSPBFSolver BFSolver;
    TSPBnBSolver BnBSolver;
    TSPDPSolver DPSolver;

    TimeTester testers[3];
    for (auto &tester : testers) {
        tester.setTestNum(testNum);
    }

    ofstream output;
    output.open(file);
    output.imbue(locale("pl_PL.utf8"));
    output << fixed << setprecision(12);
    output << "Size;BF;BnB;DP" << endl;

    for (int i = 1; i <= maxSize; ++i) {
        testers[0].setPreTestFn(bind(&TSPBFSolver::random, &BFSolver, i));
        testers[1].setPreTestFn(bind(&TSPBnBSolver::random, &BnBSolver, i));
        testers[2].setPreTestFn(bind(&TSPDPSolver::random, &DPSolver, i));
        output << i << ";";
        cout << "N = " << i << endl;

        if (i <= BFMax) {
            cout << "BF:" << endl;
            testers[0].setTestFn(bind(&TSPBFSolver::solve, &BFSolver));
            output << testers[0].test() << ";";
        } else {
            output << ";";
        }

        if (i <= BnBMax) {
            cout << "BnB:" << endl;
            testers[1].setTestFn(bind(&TSPBnBSolver::solve, &BnBSolver));
            output << testers[1].test() << ";";
        } else {
            output << ";";
        }

        if (i <= DPMax) {
            cout << "DP:" << endl;
            testers[2].setTestFn(bind(&TSPDPSolver::solve, &DPSolver));
            output <<testers[2].test() << ";";
        } else {
            output << ";";
        }

        output << endl;
    }

    output.close();
}

/**
 * Provides REPL-like user interface for testing and using TSP algorithms.
 */
void repl() {
    TSP tsp;
    TSPBFSolver BFSolver;
    TSPBnBSolver BnBSolver;
    TSPDPSolver DPSolver;

    cout << "\nAvailable commands:\n"
            "l [file]                   - Load TSP problem from file\n"
            "r [cities]                 - Generate random problem\n"
            "p                          - Print problem\n"
            "bf                         - Brute Force algorithm\n"
            "bnb                        - Branch and Bound algorithm\n"
            "dp                         - Held-Karp algorithm (dynamic programming)\n"
            "m [file] [tests] [size]    - Time measurements\n"
            "q                          - Quit\n";

    string action;
    istringstream actionStream;
    string command;
    string parameter1;
    string parameter2;
    string parameter3;

    while (true) {
        cout << "> ";
        getline(cin, action);
        actionStream = istringstream(action);
        getline(actionStream, command, ' ');
        getline(actionStream, parameter1, ' ');
        getline(actionStream, parameter2, ' ');
        getline(actionStream, parameter3, ' ');

        if (command == "l") {
            try {
                tsp.load(parameter1);
            } catch (invalid_argument &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "r") {
            tsp.random(stoi(parameter1));
        } else if (command == "p") {
            cout << tsp.toString();
        } else if (command == "bf") {
            try {
                BFSolver.setTsp(tsp);
                cout << BFSolver.solve().toString() << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "bnb") {
            try {
                BnBSolver.setTsp(tsp);
                cout << BnBSolver.solve().toString() << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "dp") {
            try {
                DPSolver.setTsp(tsp);
                cout << DPSolver.solve().toString() << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "m") {
            measure(parameter1, stoi(parameter2), stoi(parameter3));
        } else if (command == "q") {
            break;
        }
    }
}

int main() {
    repl();

    return 0;
}
