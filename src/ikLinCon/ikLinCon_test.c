/*
Copyright (C) 2015-2017 IK4-IKERLAN

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
 * @file ikLinCon_test.c
 * 
 * @brief Class ikLinCon unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../ikLinCon/ikLinCon.h"

/*
 * Simple C Test Suite
 */

/**
 * See that the class works as expected
 */
void testNormal() {
    printf("ikLinCon_test testNormal\n");
    /* declare error code */
    int err;
    /* declare output value */
    double output;
    /* declare instance */
    ikLinCon con;
    /* declare initialisation parameters */
    ikLinConParams params;
    /* declare gain schedule key */
    double x;
    
    /* see that, by default, one gets simple feedback loop */
    ikLinCon_initParams(&params);
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 1.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 0.0, 1.0);
    if (fabs(-1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -1.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 1.0, 1.0);
    if (fabs(0.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 0.0, but returned %f\n", output);
    
    /* see that the demand transfer functions are alive */
    ikLinCon_initParams(&params);
    params.demandTfs.tfParams[0].enable = 1;
    params.demandTfs.tfParams[0].b[0] = 2.0;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 2.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 0.0, 1.0);
    if (fabs(-1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -1.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 1.0, 1.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 1.0, but returned %f\n", output);
    
    /* see that the measurement transfer functions are alive */
    ikLinCon_initParams(&params);
    params.measurementTfs.tfParams[0].enable = 1;
    params.measurementTfs.tfParams[0].b[0] = 2.0;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 1.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 0.0, 1.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -2.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 1.0, 1.0);
    if (fabs(-1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -1.0, but returned %f\n", output);
    
    /* see that the error transfer functions are alive */
    ikLinCon_initParams(&params);
    params.errorTfs.tfParams[0].enable = 1;
    params.errorTfs.tfParams[0].b[0] = 2.0;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 2.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 0.0, 1.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -2.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 1.0, 1.0);
    if (fabs(0.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 0.0, but returned %f\n", output);
    
    /* see that the post-gain transfer functions are alive */
    ikLinCon_initParams(&params);
    params.postGainTfs.tfParams[0].enable = 1;
    params.postGainTfs.tfParams[0].b[0] = 2.0;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 2.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 0.0, 1.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -2.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 1.0, 1.0);
    if (fabs(0.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 0.0, but returned %f\n", output);
        
    /* see that the demand notch filters are alive, by feeding in sine signals */
    ikLinCon_initParams(&params);
    params.demandNotches.dT = 0.001;
    params.demandNotches.notchParams[0].freq = 1.0;
    params.demandNotches.notchParams[0].dampNum = 0.0;
    params.demandNotches.notchParams[0].dampDen = 0.5;
    params.demandNotches.notchParams[0].enable = 1;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    /* this ought to get filtered out */
    int i;
    for (i = 0; i < 100000; i++) ikLinCon_step(&con, sin(0.001*i), 0.0);
    double maxOut = 0;
    for (i = 100000; i < 110000; i++) {
        output = ikLinCon_step(&con, sin(0.001*i), 0.0);
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < maxOut) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=expected output of zero amplitude, but got amplitude %f\n", maxOut);
    /* this ought to make it through */
    for (i = 0; i < 100000; i++) ikLinCon_step(&con, 0.0, sin(0.001*i));
    maxOut = 0;
    for (i = 100000; i < 110000; i++) {
        output = ikLinCon_step(&con, 0.0, sin(0.001*i));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < fabs(1.0 - maxOut)) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=expected output of unit amplitude, but got amplitude %f\n", maxOut);
    
    /* see that the measurement notch filters are alive, by feeding in sine signals */
    ikLinCon_initParams(&params);
    params.measurementNotches.dT = 0.001;
    params.measurementNotches.notchParams[0].freq = 1.0;
    params.measurementNotches.notchParams[0].dampNum = 0.0;
    params.measurementNotches.notchParams[0].dampDen = 0.5;
    params.measurementNotches.notchParams[0].enable = 1;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    /* this ought to make it through */
    for (i = 0; i < 100000; i++) ikLinCon_step(&con, sin(0.001*i), 0.0);
    maxOut = 0;
    for (i = 100000; i < 110000; i++) {
        output = ikLinCon_step(&con, sin(0.001*i), 0.0);
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < fabs(1.0 - maxOut)) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=expected output of unit amplitude, but got amplitude %f\n", maxOut);
    /* this ought to get filtered out */
    for (i = 0; i < 100000; i++) ikLinCon_step(&con, 0.0, sin(0.001*i));
    maxOut = 0;
    for (i = 100000; i < 110000; i++) {
        output = ikLinCon_step(&con, 0.0, sin(0.001*i));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < maxOut) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=expected output of zero amplitude, but got amplitude %f\n", maxOut);
    
    /* see that the gain schedule is alive */
    ikLinCon_initParams(&params);
    x = 2.0;
    params.gainShedXVal = &x;
    params.gainSchedY[0] = 1.0;
    params.gainSchedY[1] = -1.0;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -2.0, but returned %f\n", output);
    output = ikLinCon_step(&con, 0.0, 1.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return 2.0, but returned %f\n", output);
    
    /* see that the gain schedule comes before the post-gain transfer function and after the error transfer functions */
    ikLinCon_initParams(&params);
    x = 2.0;
    params.gainShedXVal = &x;
    params.gainSchedY[0] = 1.0;
    params.gainSchedY[1] = -1.0;
    params.postGainTfs.tfParams[0].enable = 1;
    params.postGainTfs.tfParams[0].b[0] = 3.0;
    params.errorTfs.tfParams[0].enable = 1;
    params.errorTfs.tfParams[0].b[0] = 5.0;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(-30.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=step expected to return -30.0, but returned %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "post-gain value");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=getOutput expected to return 0 for post-gain value, but returned %d\n", err);
    if (fabs(-10.0-output)  > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=getOutput expected to fetch -10.0 for post-gain value, but fetched %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "error transfer functions");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=getOutput expected to return 0 for error transfer functions, but returned %d\n", err);
    if (fabs(5.0-output)  > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikLinCon_test) message=getOutput expected to fetch 5.0 for error transfer functions, but fetched %f\n", output);
        
}

/**
 * See that bad initialisation parameters result in the right error codes
 */
void testInitErrors() {
    printf("ikLinCon_test testInitErrors\n");
    /* declare error code */
    int err;
    /* declare instance */
    ikLinCon con;
    /* declare initialisation parameters */
    ikLinConParams params;
    
    /* error code -1 from demand transfer functions */
    ikLinCon_initParams(&params);
    params.demandTfs.tfParams[0].enable = 1;
    params.demandTfs.tfParams[0].a[0] = 0.0;
    err = ikLinCon_init(&con, &params);
    if (-1 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -1, but it returned %d\n", err);
    
    /* error code -2 from measurement transfer functions */
    ikLinCon_initParams(&params);
    params.measurementTfs.tfParams[0].enable = 1;
    params.measurementTfs.tfParams[0].a[0] = 0.0;
    err = ikLinCon_init(&con, &params);
    if (-2 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -2, but it returned %d\n", err);
    
    /* error code -3 from error transfer functions */
    ikLinCon_initParams(&params);
    params.errorTfs.tfParams[0].enable = 1;
    params.errorTfs.tfParams[0].a[0] = 0.0;
    err = ikLinCon_init(&con, &params);
    if (-3 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -3, but it returned %d\n", err);
    
    /* error code -4 from demand notch filters */
    ikLinCon_initParams(&params);
    params.demandNotches.dT = 0.0;
    err = ikLinCon_init(&con, &params);
    if (-4 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -4, but it returned %d\n", err);
    
    /* error code -5 from measurement notch filters */
    ikLinCon_initParams(&params);
    params.measurementNotches.dT = 0.0;
    err = ikLinCon_init(&con, &params);
    if (-5 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -5, but it returned %d\n", err);
    
    /* error code -6 from post-gain transfer function */
    ikLinCon_initParams(&params);
    params.postGainTfs.tfParams[0].enable = 1;
    params.postGainTfs.tfParams[0].a[0] = 0.0;
    err = ikLinCon_init(&con, &params);
    if (-6 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -6, but it returned %d\n", err);
    
    /* error code -7 from gain schedule */
    ikLinCon_initParams(&params);
    params.gainSchedX[0] = 1.0;
    params.gainSchedX[1] = -1.0;
    err = ikLinCon_init(&con, &params);
    if (-7 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -7, but it returned %d\n", err);
    
    /* error code -8 for enable presets */
    ikLinCon_initParams(&params);
    params.configN = IKLINCON_MAXNCONFIG + 1;
    err = ikLinCon_init(&con, &params);
    if (-8 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -8, but it returned %d\n", err);
    ikLinCon_initParams(&params);
    params.configN = -1;
    err = ikLinCon_init(&con, &params);
    if (-8 != err)  printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikLinCon_test) message=expected init tor return error code -8, but it returned %d\n", err);
    
    
}

/**
 * See that signals are accessible as specified
 */
void testGetOutput() {
    printf("ikLinCon_test testGetOutput\n");
    /* declare error code */
    int err;
    /* declare output value */
    double output;
    /* declare intermediate value */
    double inter;
    /* declare instance */
    ikLinCon con;
    /* declare initialisation parameters */
    ikLinConParams params;
    
    /* see that signals are accessible */
    ikLinCon_initParams(&params);
    double x = 1.0;
    params.gainShedXVal = &x;
    params.errorTfs.tfParams[0].enable = 1;
    params.errorTfs.tfParams[0].b[0] = 2.0;
    params.demandTfs.tfParams[0].enable = 1;
    params.demandTfs.tfParams[0].b[0] = -1.0;
    params.measurementTfs.tfParams[0].enable = 1;
    params.measurementTfs.tfParams[0].b[0] = -4.0;
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    output = ikLinCon_step(&con, -8.0, -4.0);
    if (fabs(-16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=step expected to return -16.0, but returned %f\n", output);
    
    /* demand */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "demand");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-8.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -8.0 for demand, but fetched %f\n", inter);
    }
    
    /* filtered demand */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "filtered demand");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(8.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch 8.0 for filtered demand, but fetched %f\n", inter);
    }
    
    /* measurement */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "measurement");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-4.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -4.0 for measurement, but fetched %f\n", inter);
    }
    
    /* filtered measurement */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "filtered measurement");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(16.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch 16.0 for filtered measurement, but fetched %f\n", inter);
    }
    
    /* error */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "error");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-8.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -8.0 for error, but fetched %f\n", inter);
    }
    
    /* gain schedule output */
    inter = 0.0;
    x = 200.0; /* this makes the gain 200, but we want the output as it was after the step, with gain 1 */
    err = ikLinCon_getOutput(&con, &inter, "gain schedule");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-16.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -16.0 for gain schedule, but fetched %f\n", inter);
    }
    
    /* control action */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "control action");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-16.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -16.0 for control action, but fetched %f\n", inter);
    }
    
    /* demand transfer functions */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "demand transfer functions");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(8.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch 8.0 for demand transfer functions, but fetched %f\n", inter);
    }
    
    /* demand transfer functions>0 */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "demand transfer functions>0");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(8.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch 8.0 for demand transfer functions>0, but fetched %f\n", inter);
    }
    
    /* measurement transfer functions>0 */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "measurement transfer functions>0");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(16.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch 16.0 for measurement transfer functions>0, but fetched %f\n", inter);
    }
    
    /* measurement transfer functions */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "measurement transfer functions");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(16.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch 16.0 for measurement transfer functions, but fetched %f\n", inter);
    }
    
    /* error transfer functions */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "error transfer functions");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-16.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -16.0 for error transfer functions, but fetched %f\n", inter);
    }
    
    /* error transfer functions>0 */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "error transfer functions>0");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-16.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -16.0 for error transfer functions>0, but fetched %f\n", inter);
    }
    
    /* error transfer functions>1 */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "error transfer functions>1");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-8.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -8.0 for error transfer functions>1, but fetched %f\n", inter);
    }
    
    /* demand notch filters */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "demand notch filters");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-8.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -8.0 for demand notch filters, but fetched %f\n", inter);
    }
    
    /* measurement notch filters */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "measurement notch filters");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-4.0-inter) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -4.0 for measurement notch filters, but fetched %f\n", inter);
    }
    
    /* post-gain tansfer function */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "post-gain transfer functions");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0, but returned %d\n", err);
    } else {
        if (fabs(-16.0-inter)  > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -16.0 for post-gain transfer functions, but fetched %f\n", inter);
    }
    
    /* post-gain value */
    inter = 0.0;
    err = ikLinCon_getOutput(&con, &inter, "post-gain value");
    if (err) {
        printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to return 0 for post-gain value, but returned %d\n", err);
    } else {
        if (fabs(-16.0-inter)  > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikLinCon_test) message=getOutput expected to fetch -16.0 for post-gain value, but fetched %f\n", inter);
    }
    
}

/**
 * See that getOutput returns the right error codes
 */
void testGetOutputErrors() {
    printf("ikLinCon_test testGetOutputErrors\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikLinCon con;
    /* declare initialisation parameters */
    ikLinConParams params;
    
    /* -1 for invalid signal names */
    ikLinCon_initParams(&params);
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutputErrors (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    err = ikLinCon_getOutput(&con, &output, "errors");
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testGetOutputErrors (ikLinCon_test) message=getOutput expected to return -1, but returned %d\n", err);
    err = ikLinCon_getOutput(&con, &output, "errors>0");
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=testGetOutputErrors (ikLinCon_test) message=getOutput expected to return -2, but returned %d\n", err);
    
}

void testConfigurations() {
    printf("ikLinCon_test testConfigurations\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare output extremum */
    double maxOutput;
    /* declare counter */
    int i;
    /* declare instance */
    ikLinCon con;
    /* declare initialisation parameters */
    ikLinConParams params;
    
    /* prepare initialisation parameters */
    ikLinCon_initParams(&params);
    /* 6 configurations */
    int config = 0;
    params.configN = 6;
    params.config = &config;
    /* dT is 0.001 */
    params.demandNotches.dT = 0.001;
    params.measurementNotches.dT = 0.001;
    
    /* a notch filter at 1.0 rad/s for the demand */
    /* enabled in configuration 0 */
    params.demandNotchesEnable[0][0] = 1;
    params.demandNotches.notchParams[0].freq = 1.0;
    params.demandNotches.notchParams[0].dampNum = 0.0;
    params.demandNotches.notchParams[0].dampDen = 0.1;
    /* a notch filter at 0.1 rad/s for the demand */
    /* enabled in configuration 1 */
    params.demandNotchesEnable[1][1] = 1;
    params.demandNotches.notchParams[1].freq = 0.1;
    params.demandNotches.notchParams[1].dampNum = 0.0;
    params.demandNotches.notchParams[1].dampDen = 0.1;
    /* a gain of 2 for the demand */
    /* enabled in configuration 0 */
    params.demandTfsEnable[0][1] = 1;
    params.demandTfs.tfParams[1].b[0] = 2.0;
    /* a gain of 4 for the demand */
    /* enabled in configuration 1 */
    params.demandTfsEnable[1][2] = 1;
    params.demandTfs.tfParams[2].b[0] = 4.0;
    
    /* a notch filter at 1.0 rad/s for the measurement */
    /* enabled in configuration 2 */
    params.measurementNotchesEnable[2][0] = 1;
    params.measurementNotches.notchParams[0].freq = 1.0;
    params.measurementNotches.notchParams[0].dampNum = 0.0;
    params.measurementNotches.notchParams[0].dampDen = 0.1;
    /* a notch filter at 0.1 rad/s for the measurement */
    /* enabled in configuration 3 */
    params.measurementNotchesEnable[3][1] = 1;
    params.measurementNotches.notchParams[1].freq = 0.1;
    params.measurementNotches.notchParams[1].dampNum = 0.0;
    params.measurementNotches.notchParams[1].dampDen = 0.1;
    /* a gain of 2 for the measurement */
    /* enabled in configuration 2 */
    params.measurementTfsEnable[2][1] = 1;
    params.measurementTfs.tfParams[1].b[0] = 2.0;
    /* a gain of 4 for the measurement */
    /* enabled in configuration 3 */
    params.measurementTfsEnable[3][2] = 1;
    params.measurementTfs.tfParams[2].b[0] = 4.0;
    
    /* a gain of 2 for the error */
    /* enabled in configuration 4 */
    params.errorTfsEnable[4][1] = 1;
    params.errorTfs.tfParams[1].b[0] = 2.0;
    /* a gain of 4 for the error */
    /* enabled in configuration 5 */
    params.errorTfsEnable[5][2] = 1;
    params.errorTfs.tfParams[2].b[0] = 4.0;
        
    /* initialise instance */
    err = ikLinCon_init(&con, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    
    /* see that configuration 0 has a notch filter at 1.0 rad/s and a gain of 2 for the demand */
    config = 0;
    ikLinCon_step(&con, 32.0, -32.0);
    err = ikLinCon_getOutput(&con, &output, "measurement transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for demand measurement functions, but returned %d\n", err);
    if (fabs(-32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected measurement transfer functions output to be -32.0, but it is %f\n", output);
    /* at frequency 1.0 rad/s, it gets filtered out */
    for (i = 0; i < 100000; i++) {
        ikLinCon_step(&con, sin(0.001*i), 0.0);
    }
    maxOutput = 0.0;
    for (i = 100000; i < 110000; i++) {
        output = ikLinCon_step(&con, sin(0.001*i), 0.0);
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < maxOutput) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 1 rad/s at the demand expected to be filtered out, but it appeared at the output with amplitude %f\n", maxOutput);
    /* at frequency 0.1 rad/s, it gets through */
    for (i = 0; i < 1000000; i++) {
        ikLinCon_step(&con, sin(0.0001*i), 0.0);
    }
    maxOutput = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        output = ikLinCon_step(&con, sin(0.0001*i), 0.0);
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < fabs(maxOutput - 2.0)) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 0.1 rad/s at the demand expected to make it through with amplitude 2.0, but it made it with amplitude %f\n", maxOutput);
    
    /* see that configuration 1 has a notch filter at 0.1 rad/s and a gain of 4 for the demand */
    config = 1;
    ikLinCon_step(&con, 32.0, -32.0);
    err = ikLinCon_getOutput(&con, &output, "measurement transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for demand measurement functions, but returned %d\n", err);
    if (fabs(-32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected measurement transfer functions output to be -32.0, but it is %f\n", output);
    /* at frequency 1.0 rad/s, it gets through */
    for (i = 0; i < 1000000; i++) {
        ikLinCon_step(&con, sin(0.001*i), 0.0);
    }
    maxOutput = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        output = ikLinCon_step(&con, sin(0.001*i), 0.0);
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < fabs(maxOutput - 4.0)) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 1.0 rad/s at the demand expected to make it through with amplitude 4.0, but it made it with amplitude %f\n", maxOutput);
    /* at frequency 0.1 rad/s, it gets filtered out */
    for (i = 0; i < 1000000; i++) {
        ikLinCon_step(&con, sin(0.0001*i), 0.0);
    }
    maxOutput = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        output = ikLinCon_step(&con, sin(0.0001*i), 0.0);
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < maxOutput) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 0.1 rad/s at the demand expected to be filtered out, but it appeared at the output with amplitude %f\n", maxOutput);
    
    /* see that configuration 2 has a notch filter at 1.0 rad/s and a gain of 2 for the measurement */
    config = 2;
    ikLinCon_step(&con, 32.0, -32.0);
    err = ikLinCon_getOutput(&con, &output, "demand transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for demand transfer functions, but returned %d\n", err);
    if (fabs(32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected demand transfer functions output to be 32.0, but it is %f\n", output);
    /* at frequency 1.0 rad/s, it gets filtered out */
    for (i = 0; i < 1000000; i++) {
        ikLinCon_step(&con, 0.0, sin(0.001*i));
    }
    maxOutput = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        output = ikLinCon_step(&con, 0.0, sin(0.001*i));
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < maxOutput) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 1.0 rad/s at the measurement expected to be filtered out, but it appeared at the output with amplitude %f\n", maxOutput);
    /* at frequency 0.1 rad/s, it gets through */
    for (i = 0; i < 1000000; i++) {
        ikLinCon_step(&con, 0.0, sin(0.0001*i));
    }
    maxOutput = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        output = ikLinCon_step(&con, 0.0, sin(0.0001*i));
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < fabs(maxOutput - 2.0)) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 0.1 rad/s at the measurement expected to make it through with amplitude 2.0, but it made it with amplitude %f\n", maxOutput);
    
    /* see that configuration 3 has a notch filter at 0.1 rad/s and a gain of 4 for the measurement */
    config = 3;
    ikLinCon_step(&con, 32.0, -32.0);
    err = ikLinCon_getOutput(&con, &output, "demand transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for demand transfer functions, but returned %d\n", err);
    if (fabs(32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected demand transfer functions output to be 32.0, but it is %f\n", output);
    /* at frequency 1.0 rad/s, it gets through */
    for (i = 0; i < 1000000; i++) {
        ikLinCon_step(&con, 0.0, sin(0.001*i));
    }
    maxOutput = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        output = ikLinCon_step(&con, 0.0, sin(0.001*i));
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < fabs(maxOutput - 4.0)) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 1.0 rad/s at the measurement expected to make it through with amplitude 4.0, but it made it with amplitude %f\n", maxOutput);
    /* at frequency 0.1 rad/s, it gets filtered out */
    for (i = 0; i < 1000000; i++) {
        ikLinCon_step(&con, 0.0, sin(0.0001*i));
    }
    maxOutput = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        output = ikLinCon_step(&con, 0.0, sin(0.0001*i));
        if (fabs(output) > maxOutput) maxOutput = fabs(output);
    }
    if (0.001 < maxOutput) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=sine of 0.1 rad/s at the measurement expected to be filtered out, but it appeared at the output with amplitude %f\n", maxOutput);
    
    /* see that configuration 4 has a gain of 2 for the error */
    config = 4;
    ikLinCon_step(&con, 32.0, -32.0);
    err = ikLinCon_getOutput(&con, &output, "demand transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for demand transfer functions, but returned %d\n", err);
    if (fabs(32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected demand transfer functions output to be 32.0, but it is %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "measurement transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for measurement transfer functions, but returned %d\n", err);
    if (fabs(-32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected measurement transfer functions output to be -32.0, but it is %f\n", output);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(output - 2.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=step expected to return 2.0, but it returned %f\n", output);
    
    /* see that configuration 5 has a gain of 4 for the error */
    config = 5;
    ikLinCon_step(&con, 32.0, -32.0);
    err = ikLinCon_getOutput(&con, &output, "demand transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for demand transfer functions, but returned %d\n", err);
    if (fabs(32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected demand transfer functions output to be 32.0, but it is %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "measurement transfer functions");
    if (err)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=getOutput expected to return 0 for measurement transfer functions, but returned %d\n", err);
    if (fabs(-32.0-output) > 1e-9)  printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=expected measurement transfer functions output to be -32.0, but it is %f\n", output);
    output = ikLinCon_step(&con, 1.0, 0.0);
    if (fabs(output - 4.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testConfigurations (ikLinCon_test) message=step expected to return 4.0, but it returned %f\n", output);
        
}

void testSaturation() {
    printf("ikLinCon_test testSaturation\n");
    
    /* allocate controller */
    ikLinCon con;
    
    /* allocate initialisation parameters */
    ikLinConParams param;
    
    /* allocate error code */
    int err;
    
    /* allocate output value */
    double output;
    
    /* allocate saturation limits */
    double maxOut, minOut, maxIn, minIn;
    
    /* initialise controller with the default static gains of 1 and enabled saturated limits */
    ikLinCon_initParams(&param);
    param.maxControlAction = &maxOut;
    param.minControlAction = &minOut;
    param.maxPostGainValue = &maxIn;
    param.minPostGainValue = &minIn;
    err = ikLinCon_init(&con, &param);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=init expected to return 0, but returned %d\n", err);
    
    /* see that saturation is applied at the post-gain transfer functions */
    maxOut = 3.0;
    minOut = -4.0;
    maxIn = 5.0;
    minIn = -6.0;
    output = ikLinCon_step(&con, 1100.0, 1000.0);
    if (fabs(3.0 - output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=step expected to return 3.0, but returned %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "post-gain transfer functions>1");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to return 0 for post-gain transfer functions>1, but returned %d\n", err);
    if (fabs(5.0 - output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to fetch 5.0 for post-gain transfer functions>1, but fetched %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "post-gain value");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to return 0 for post-gain value, but returned %d\n", err);
    if (fabs(100.0 - output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to fetch 100.0 for post-gain value, but fetched %f\n", output);
    
    output = ikLinCon_step(&con, 1000.0, 1100.0);
    if (fabs(-4.0 - output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=step expected to return -4.0, but returned %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "post-gain transfer functions>1");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to return 0 for post-gain transfer functions>1, but returned %d\n", err);
    if (fabs(-6.0 - output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to fetch -6.0 for post-gain transfer functions>1, but fetched %f\n", output);
    err = ikLinCon_getOutput(&con, &output, "post-gain value");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to return 0 for post-gain value, but returned %d\n", err);
    if (fabs(-100.0 - output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSaturation (ikLinCon_test) message=getOutput expected to fetch -100.0 for post-gain value, but fetched %f\n", output);
        
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikLinCon_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testNormal (ikLinCon_test)\n");
    testNormal();
    printf("%%TEST_FINISHED%% time=0 testNormal (ikLinCon_test) \n");

    printf("%%TEST_STARTED%% testInitErrors (ikLinCon_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 testInitErrors (ikLinCon_test) \n");

    printf("%%TEST_STARTED%% testGetOutput (ikLinCon_test)\n");
    testGetOutput();
    printf("%%TEST_FINISHED%% time=0 testGetOutput (ikLinCon_test) \n");

    printf("%%TEST_STARTED%% testGetOutputErrors (ikLinCon_test)\n");
    testGetOutputErrors();
    printf("%%TEST_FINISHED%% time=0 testGetOutputErrors (ikLinCon_test) \n");

    printf("%%TEST_STARTED%% testConfigurations (ikLinCon_test)\n");
    testConfigurations();
    printf("%%TEST_FINISHED%% time=0 testConfigurations (ikLinCon_test) \n");

    printf("%%TEST_STARTED%% testSaturation (ikLinCon_test)\n");
    testSaturation();
    printf("%%TEST_FINISHED%% time=0 testSaturation (ikLinCon_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
