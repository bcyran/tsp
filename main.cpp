#include <iostream>
#include <functional>
#include <iomanip>
#include <fstream>
#include "TSPExact.h"
#include "TimeTester.h"

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
    int BFMax = 11;
    int BnBMax = 15;
    int DPMax = 19;

    TSPExact tsp;
    TimeTester tester;
    tester.setTestNum(testNum);

    ofstream output;
    output.open(file);
    output.imbue(locale("pl_PL.utf8"));
    output << fixed << setprecision(12);
    output << "Size;BF;BnB;DP" << endl;

    for (int i = 1; i <= maxSize; ++i) {
        tester.setPreTestFn(bind(&TSPExact::random, &tsp, i));
        output << i << ";";
        cout << "N = " << i << endl;

        if (i <= BFMax) {
            cout << "BF:" << endl;
            tester.setTestFn(bind(&TSPExact::solveBF, &tsp));
            output << tester.test() << ";";
        } else {
            output << ";";
        }

        if (i <= BnBMax) {
            cout << "BnB:" << endl;
            tester.setTestFn(bind(&TSPExact::solveBnB, &tsp));
            output << tester.test() << ";";
        } else {
            output << ";";
        }

        if (i <= DPMax) {
            cout << "DP:" << endl;
            tester.setTestFn(bind(&TSPExact::solveDP, &tsp));
            output << tester.test() << ";";
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
    TSPExact tsp;

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
                cout << tsp.solveBF().toString() << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "bnb") {
            try {
                cout << tsp.solveBnB().toString() << endl;
            } catch (runtime_error &e) {
                cerr << e.what() << endl;
            }
        } else if (command == "dp") {
            try {
                cout << tsp.solveDP().toString() << endl;
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
