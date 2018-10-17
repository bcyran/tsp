//
// Created by bazyli on 17.10.18.
//

#ifndef C_TIMETESTER_H
#define C_TIMETESTER_H

#include <functional>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef function<void(void)> fn;

/**
 * Runs specified function many times and calculates average time of execution.
 */
class TimeTester {

    /** Pointer to tested function. */
    fn testFn;

    /** Pointer to function to run before every test (eg. preparation). */
    fn preTestFn;

    /** Pointer to function to run after every test (eg. cleanup). */
    fn postTestFn;

    /** Number of tests to run. */
    int testNum = 0;

public:

    TimeTester();

    TimeTester(fn testFn, fn preTestFn, fn postTestFn, int testNum);

    double test();

    void setTestFn(fn testFn);

    void setPreTestFn(fn preTestFn);

    void setPostTestFn(fn postTestFn);

    void setTestNum(int testNum);

};

#endif //C_TIMETESTER_H
