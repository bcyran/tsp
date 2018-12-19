#include <iostream>
#include <functional>
#include <iomanip>
#include <fstream>
#include "TSPSimAnnealingSolver.h"
#include "TSPTabuSolver.h"
#include "TSPSolver.h"
#include "TSPBFSolver.h"
#include "TSPBnBSolver.h"
#include "TSPDPSolver.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

/**
 * Provides REPL-like user interface for testing and using TSP algorithms.
 */
void repl() {
    TSP tsp;
    TSPBFSolver BFSolver;
    TSPBnBSolver BnBSolver;
    TSPDPSolver DPSolver;
    TSPTabuSolver TSolver;
    TSPSimAnnealingSolver SASolver;

    cout << "\nAvailable commands:\n"
            "l [file]                   - Load TSP problem from file\n"
            "r [cities]                 - Generate random problem\n"
            "p                          - Print problem\n"
            "bf                         - Brute Force algorithm\n"
            "bnb                        - Branch and Bound algorithm\n"
            "dp                         - Held-Karp algorithm (dynamic programming)\n"
            "t                          - Tabu Search\n"
            "sa                         - Simulated Annealing\n"
            "q                          - Quit\n";

    string action;
    istringstream actionStream;
    string command;
    string parameter1;
    string parameter2;
    string parameter3;

    Path result;
    high_resolution_clock::time_point timeStart;
    high_resolution_clock::time_point timeStop;
    duration<double> time{};

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
                timeStart = high_resolution_clock::now();
                result = BFSolver.solve();
                timeStop = high_resolution_clock::now();
                time = duration_cast<duration<double>>(timeStop - timeStart);
                cout << "Solution: " << result.toString() << endl << "Duration: " << time.count() << " s" << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "bnb") {
            try {
                BnBSolver.setTsp(tsp);
                timeStart = high_resolution_clock::now();
                result = BnBSolver.solve();
                timeStop = high_resolution_clock::now();
                time = duration_cast<duration<double>>(timeStop - timeStart);
                cout << "Solution: " << result.toString() << endl << "Duration: " << time.count() << " s" << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "dp") {
            try {
                DPSolver.setTsp(tsp);
                timeStart = high_resolution_clock::now();
                result = DPSolver.solve();
                timeStop = high_resolution_clock::now();
                time = duration_cast<duration<double>>(timeStop - timeStart);
                cout << "Solution: " << result.toString() << endl << "Duration: " << time.count() << " s" << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "t") {
            try {
                TSolver.setTsp(tsp);
                timeStart = high_resolution_clock::now();
                result = TSolver.solve();
                timeStop = high_resolution_clock::now();
                time = duration_cast<duration<double>>(timeStop - timeStart);
                cout << "Solution: " << result.toString() << endl << "Duration: " << time.count() << " s" << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "sa") {
            try {
                SASolver.setTsp(tsp);
                timeStart = high_resolution_clock::now();
                result = SASolver.solve();
                timeStop = high_resolution_clock::now();
                time = duration_cast<duration<double>>(timeStop - timeStart);
                cout << "Solution: " << result.toString() << endl << "Duration: " << time.count() << " s" << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "q") {
            break;
        }
    }
}

int main() {
    repl();

    return 0;
}
