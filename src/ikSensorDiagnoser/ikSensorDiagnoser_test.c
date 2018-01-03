/*
Copyright (C) 2017 IK4-IKERLAN

This file is part of OpenWitcon.
 
OpenWitcon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
OpenWitcon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with OpenWitcon. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file ikSensorDiagnoser_test.c
 * 
 * @brief Class ikSensorDiagnoser unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include "ikSensorDiagnoser.h"

/*
 * Simple C Test Suite
 */

/**
 * test default behaviour
 */
void testDefault() {
    printf("ikSensorDiagnoser_test testDefault\n");
    /* allocate instance */
    ikSensorDiagnoser sd;
    
    /* allocate initialisation parameters */
    ikSensorDiagnoserParams param;
    
    /* allocate error code */
    int err;
    
    /* allocate inputs */
    double signals[3];
    
    /* allocate output */
    int ok[3] = {0};
    
    /* allocate a counter */
    int i;
    
    /* initialise instance with default parameters */
    ikSensorDiagnoser_initParams(&param);
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    
    /* see that three equal signals return all ok a few times */
    for(i = 0; i < 3; i++) signals[i] = 3.0;
    ikSensorDiagnoser_step(&sd, ok, signals);
    for(i = 0; i < 3; i++) {
        if (!ok[i]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor %d, but returned %d\n", i, ok[i]);
    }
    for(i = 0; i < 3; i++) signals[i] = -18.0;
    ikSensorDiagnoser_step(&sd, ok, signals);
    for(i = 0; i < 3; i++) {
        if (!ok[i]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor %d, but returned %d\n", i, ok[i]);
    }
    for(i = 0; i < 3; i++) signals[i] = 0.0;
    ikSensorDiagnoser_step(&sd, ok, signals);
    for(i = 0; i < 3; i++) {
        if (!ok[i]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor %d, but returned %d\n", i, ok[i]);
    }
    
    /* see that three signals withing 1.0 of each other return all ok a few times*/
    signals[0] = 3.0;
    signals[1] = 3.2;
    signals[2] = 2.9;
    ikSensorDiagnoser_step(&sd, ok, signals);
    for(i = 0; i < 3; i++) {
        if (!ok[i]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor %d, but returned %d\n", i, ok[i]);
    }
    ikSensorDiagnoser_step(&sd, ok, signals);
    for(i = 0; i < 3; i++) {
        if (!ok[i]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor %d, but returned %d\n", i, ok[i]);
    }
    ikSensorDiagnoser_step(&sd, ok, signals);
    for(i = 0; i < 3; i++) {
        if (!ok[i]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor %d, but returned %d\n", i, ok[i]);
    }
    
    /* see that the outlier gets picked out in one step */
    ikSensorDiagnoser_initParams(&param);
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 7.0;
    signals[1] = 3.2;
    signals[2] = 2.9;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    
    ikSensorDiagnoser_initParams(&param);
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 3.0;
    signals[1] = -5.2;
    signals[2] = 2.9;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    
    ikSensorDiagnoser_initParams(&param);
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 3.0;
    signals[1] = 3.2;
    signals[2] = 12.9;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 2, but returned %d\n", ok[2]);
    
    /* see that two sensor failures in sequence results in all deemed faulty */
    ikSensorDiagnoser_initParams(&param);
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 5.0;
    signals[1] = 5.1;
    signals[2] = 5.2;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    signals[0] = 0.0;
    signals[1] = 5.1;
    signals[2] = 5.2;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    signals[0] = 0.0;
    signals[1] = 0.0;
    signals[2] = 5.2;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 0, but returned %d\n", ok[0]);
    if (ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 1, but returned %d\n", ok[1]);
    if (ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 2, but returned %d\n", ok[2]);
    signals[0] = 0.0;
    signals[1] = 0.0;
    signals[2] = 0.2;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 0, but returned %d\n", ok[0]);
    if (ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 1, but returned %d\n", ok[1]);
    if (ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 2, but returned %d\n", ok[2]);
    
    /* see that a faulty sensor can recover */
    ikSensorDiagnoser_initParams(&param);
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 2.0;
    signals[1] = 5.1;
    signals[2] = 5.2;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    signals[0] = 5.0;
    signals[1] = 5.1;
    signals[2] = 5.2;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    
}

/**
 * test tolerance setting
 */
void testTolerance() {
    printf("ikSensorDiagnoser_test testTolerance\n");
    /* allocate instance */
    ikSensorDiagnoser sd;
    
    /* allocate initialisation parameters */
    ikSensorDiagnoserParams param;
    
    /* allocate error code */
    int err;
    
    /* allocate inputs */
    double signals[3];
    
    /* allocate output */
    int ok[3] = {0};
    
    /* try tolerance at 10.0 */
    ikSensorDiagnoser_initParams(&param);
    param.tolerance = 10.0;
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 0.0;
    signals[1] = 9.9;
    signals[2] = 9.9;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    signals[0] = 0.0;
    signals[1] = 10.1;
    signals[2] = 10.1;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (ok[0]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    
    
    /* try tolerance at 100.0 */
    ikSensorDiagnoser_initParams(&param);
    param.tolerance = 100.0;
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 0.0;
    signals[1] = 99.9;
    signals[2] = 99.9;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    signals[0] = 0.0;
    signals[1] = 100.1;
    signals[2] = 100.1;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (ok[0]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testTolerance (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    
}

/**
 * test setting of step number to fault
 */
void testSteps() {
    printf("ikSensorDiagnoser_test testSteps\n");
    /* allocate instance */
    ikSensorDiagnoser sd;
    
    /* allocate initialisation parameters */
    ikSensorDiagnoserParams param;
    
    /* allocate error code */
    int err;
    
    /* allocate inputs */
    double signals[3];
    
    /* allocate output */
    int ok[3] = {0};
    
    /* try 3 steps */
    ikSensorDiagnoser_initParams(&param);
    param.nStepsToFault = 3;
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 0.0;
    signals[1] = 0.0;
    signals[2] = 1.1;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (ok[2]) printf("%%TEST_FAILED%% time=0 testname=testSteps (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 2, but returned %d\n", ok[2]);
    
}

/**
 * test the getOutput method
 */
void testGetOutput() {
    printf("ikSensorDiagnoser_test testGetOutput\n");
    /* allocate instance */
    ikSensorDiagnoser sd;
    
    /* allocate initialisation parameters */
    ikSensorDiagnoserParams param;
    
    /* allocate error code */
    int err;
    
    /* allocate inputs */
    double signals[3];
    
    /* allocate output */
    int ok[3] = {0};
    
    /* see that getoutput does not change states */
    ikSensorDiagnoser_initParams(&param);
    param.nStepsToFault = 3;
    err = ikSensorDiagnoser_init(&sd, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=init expected to return 0, but returned %d\n", err);
    signals[0] = 0.0;
    signals[1] = 0.0;
    signals[2] = 1.1;
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    ok[0] = 0;
    ok[1] = 0;
    ok[2] = 0;
    ikSensorDiagnoser_getOutput(&sd, ok);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (!ok[2]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 2, but returned %d\n", ok[2]);
    ikSensorDiagnoser_step(&sd, ok, signals);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (ok[2]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 2, but returned %d\n", ok[2]);
    ok[0] = 0;
    ok[1] = 0;
    ok[2] = 1;
    ikSensorDiagnoser_getOutput(&sd, ok);
    if (!ok[0]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 0, but returned %d\n", ok[0]);
    if (!ok[1]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 1 for sensor 1, but returned %d\n", ok[1]);
    if (ok[2]) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikSensorDiagnoser_test) message=step expected to return 0 for sensor 2, but returned %d\n", ok[2]);
    
}

/**
 * bad parameters result in the right error codes
 */
void testInitErrors() {
    printf("ikSensorDiagnoser_test testInitErrors\n");
    /* allocate instance */
    ikSensorDiagnoser sd;
    
    /* allocate initialisation parameters */
    ikSensorDiagnoserParams param;
    
    /* allocate error code */
    int err;
    
    /* -1 for bad n */
    ikSensorDiagnoser_initParams(&param);
    param.nStepsToFault = 0;
    err = ikSensorDiagnoser_init(&sd, &param);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikSensorDiagnoser_test) message=init expected to return -1, but returned %d\n", err);
        
    /* -2 for bad tol */
    ikSensorDiagnoser_initParams(&param);
    param.tolerance = 0.0;
    err = ikSensorDiagnoser_init(&sd, &param);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikSensorDiagnoser_test) message=init expected to return -2, but returned %d\n", err);
    
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikSensorDiagnoser_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testDefault (ikSensorDiagnoser_test)\n");
    testDefault();
    printf("%%TEST_FINISHED%% time=0 testDefault (ikSensorDiagnoser_test) \n");

    printf("%%TEST_STARTED%% testTolerance (ikSensorDiagnoser_test)\n");
    testTolerance();
    printf("%%TEST_FINISHED%% time=0 testTolerance (ikSensorDiagnoser_test) \n");

    printf("%%TEST_STARTED%% testSteps (ikSensorDiagnoser_test)\n");
    testSteps();
    printf("%%TEST_FINISHED%% time=0 testSteps (ikSensorDiagnoser_test) \n");

    printf("%%TEST_STARTED%% testGetOutput (ikSensorDiagnoser_test)\n");
    testGetOutput();
    printf("%%TEST_FINISHED%% time=0 testGetOutput (ikSensorDiagnoser_test) \n");

    printf("%%TEST_STARTED%% testInitErrors (ikSensorDiagnoser_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 testInitErrors (ikSensorDiagnoser_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
