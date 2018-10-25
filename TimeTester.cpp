//
// Created by bazyli on 17.10.18.
//

#include "TimeTester.h"
#include <iostream>

/**
 * Creates empty TimeTester object.
 */
TimeTester::TimeTester() = default;

/**
 * Creates TimeTester object with given settings.
 *
 * @param testFn Function to test.
 * @param preTestFn Function to run before every test.
 * @param postTestFn Function to run after every test.
 * @param testNum Number of tests.
 */
TimeTester::TimeTester(fn testFn, fn preTestFn, fn postTestFn, int testNum)
        : testFn(move(testFn)), preTestFn(move(preTestFn)), postTestFn(move(postTestFn)), testNum(testNum) {}

/**
 * Runs specified number of test and calculates average time of execution in seconds.
 *
 * @return Average time of function execution in seconds.
 */
double TimeTester::test() {
    if (!testFn) return -1;

    high_resolution_clock::time_point timeStart;
    high_resolution_clock::time_point timeStop;
    duration<double> time{};
    double sum = 0;

    // Run tests
    for (int i = 0; i < testNum; ++i) {
        cout << "Test: " << i + 1 << "/" << testNum << endl;

        if (preTestFn) preTestFn();
        timeStart = high_resolution_clock::now();
        testFn();
        timeStop = high_resolution_clock::now();
        if (postTestFn) postTestFn();

        time = duration_cast<duration<double>>(timeStop - timeStart);
        sum += time.count();
    }

    return sum / testNum;
}

/**
 * Setter for tested function.
 *
 * @param testFn Pointer to the function.
 */
void TimeTester::setTestFn(fn testFn) {
    TimeTester::testFn = move(testFn);
}

/**
 * Setter for function running before test.
 *
 * @param preTestFn Pointer to the function.
 */
void TimeTester::setPreTestFn(fn preTestFn) {
    TimeTester::preTestFn = move(preTestFn);
}

/**
 * Setter to function running after test.
 *
 * @param postTestFn Pointer to the function.
 */
void TimeTester::setPostTestFn(fn postTestFn) {
    TimeTester::postTestFn = move(postTestFn);
}

/**
 * Setter for number of tests.
 *
 * @param testNum Pointer to the function.
 */
void TimeTester::setTestNum(int testNum) {
    TimeTester::testNum = testNum;
    cout << this->testNum << endl;
}
