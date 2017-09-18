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
 * @file ikNotchList_test.c
 * 
 * @brief Class ikNotchList unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../ikNotchList/ikNotchList.h"

/*
 * Simple C Test Suite
 */

/**
 * See that initialisation with bad arguments results in the right error codes
 */
void testInitErrors() {
    printf("ikNotchList_test testInitErrors\n");
    /* declare error code */
    int err;
    /* declare instance */
    ikNotchList list;
    /* declare initialisation parameters */
    ikNotchListParams params;
    
    /* see that init complains about the sampling time */
    ikNotchList_initParams(&params);
    params.dT = 0.0;
    err = ikNotchList_init(&list, &params);
    if (1 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikNotchList_test) message=init expected to return error code 1, but returned %d\n", err);
    
    /* see that init complains about the frequency for the fifth notch */
    ikNotchList_initParams(&params);
    params.notchParams[4].freq = 0.0;
    err = ikNotchList_init(&list, &params);
    if (-5 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikNotchList_test) message=init expected to return error code -5, but returned %d\n", err);
    
}

/**
 * See that the class works as expected under normal conditions
 */
void testNormal() {
    printf("ikNotchList_test testNormal\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikNotchList list;
    /* declare initialisation parameters */
    ikNotchListParams params;
    
    /* see that, by default, one gets a static unit gain */
    ikNotchList_initParams(&params);
    err = ikNotchList_init(&list, &params);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=init expected to return 0, but returned %d\n", err);
    output = ikNotchList_step(&list, 2.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=step expected to return 2.0, but returned %f\n", output);
    output = ikNotchList_step(&list, -2.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=step expected to return -2.0, but returned %f\n", output);
    
    /* see that the frequency and the enable setting can be changed online, by filtering a sine of */
    /* constant frequency for a few cycles and then checking the maximum value of */
    /* the output during another cycle */
    int i;
    double input;
    int enable0 = 1;
    int enable1 = 0;
    double maxIn;
    double maxOut;
    double freq0 = 0.1;
    double freq1 = 10.0;
    ikNotchList_initParams(&params);
    params.dT = 0.001;
    params.notchParams[0].enable = 1;
    params.notchParams[0].variableEnable = &enable0;
    params.notchParams[0].variableFreq = &freq0;
    params.notchParams[0].dampNum = 0.0;
    params.notchParams[0].dampDen = 0.5;
    params.notchParams[1].enable = 1;
    params.notchParams[1].variableEnable = &enable1;
    params.notchParams[1].variableFreq = &freq1;
    params.notchParams[1].dampNum = 0.0;
    params.notchParams[1].dampDen = 0.5;
    err = ikNotchList_init(&list, &params);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=init expected to return 0, but returned %d\n", err);
    /* this ought to make it through */
    enable0 = 0;
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    maxIn = 0.0;
    maxOut = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        output = ikNotchList_step(&list, input);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < fabs(maxOut - maxIn)) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=expected unit amplitude at output, but got amplitude of %f\n", maxOut);
    /* this ought to get filtered out by the last filter */
    enable0 = 1;
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    maxIn = 0.0;
    maxOut = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        output = ikNotchList_step(&list, input);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < maxOut) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=expected zero amplitude at output, but got amplitude of %f\n", maxOut);
    /* now the last filter changes frequency and the input ought to make it through to the output */
    freq0 = 10;
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    maxIn = 0.0;
    maxOut = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        output = ikNotchList_step(&list, input);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < fabs(maxOut - maxIn)) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=expected unit amplitude at output, but got amplitude of %f\n", maxOut);
    /* now the first filter gets enabled and at the right frequency, and the input */
    /* ought to get filtered out again. */
    enable1 = 1;
    freq1 = 0.1;
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    maxIn = 0.0;
    maxOut = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        output = ikNotchList_step(&list, input);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < maxOut) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikNotchList_test) message=expected zero amplitude at output, but got amplitude of %f\n", maxOut);
    
}

/**
 * See that a bad frequency value set online is simply ignored
 */
void testBadFrequency() {
    printf("ikNotchList_test testBadFrequency\n");
    printf("ikNotchList_test testNormal\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikNotchList list;
    /* declare initialisation parameters */
    ikNotchListParams params;
    
    /* see that when the frequency is changed online to a bad value the notch */
    /* keeps working with the last valid frequency value, and that it responds */
    /* when the frequency is changed to a valid value */
    int i;
    double input;
    double maxIn;
    double maxOut;
    double freq = 0.1;
    ikNotchList_initParams(&params);
    params.dT = 0.001;
    params.notchParams[0].enable = 1;
    params.notchParams[0].variableFreq = &freq;
    params.notchParams[0].dampNum = 0.0;
    params.notchParams[0].dampDen = 0.5;
    err = ikNotchList_init(&list, &params);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=testBadFrequency (ikNotchList_test) message=init expected to return 0, but returned %d\n", err);
    /* this ought to get filtered out */
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    maxIn = 0.0;
    maxOut = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        output = ikNotchList_step(&list, input);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < maxOut) printf("%%TEST_FAILED%% time=0 testname=testBadFrequency (ikNotchList_test) message=expected zero amplitude at output, but got amplitude of %f\n", maxOut);
    /* this ought to still get filtered out, because the change of frequency is */
    /* not valid */
    freq = 0.0;
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    maxIn = 0.0;
    maxOut = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        output = ikNotchList_step(&list, input);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < maxOut) printf("%%TEST_FAILED%% time=0 testname=testBadFrequency (ikNotchList_test) message=expected zero amplitude at output, but got amplitude of %f\n", maxOut);
    /* this ought to make it through, because the change of frequency is valid */
    freq = 10.0;
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    maxIn = 0.0;
    maxOut = 0.0;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        output = ikNotchList_step(&list, input);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
    }
    if (0.001 < fabs(maxOut - maxIn)) printf("%%TEST_FAILED%% time=0 testname=testBadFrequency (ikNotchList_test) message=expected unit amplitude at output, but got amplitude of %f\n", maxOut);
    
}

/**
 * See that method getOutput gets the right values
 */
void testGetOutput() {
    printf("ikNotchList_test testGetOutput\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikNotchList list;
    /* declare initialisation parameters */
    ikNotchListParams params;
    
    /*  */
    ikNotchList_initParams(&params);
    params.dT = 0.001;
    params.notchParams[0].enable = 1;
    params.notchParams[0].freq = 0.1;
    params.notchParams[0].dampNum = 0.0;
    params.notchParams[0].dampDen = 0.5;
    err = ikNotchList_init(&list, &params);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikNotchList_test) message=init expected to return 0, but returned %d\n", err);
    /* this ought to get filtered out by the last filter, but show before that */
    int i;
    for (i = 0; i < 1000000; i++) ikNotchList_step(&list, sin(0.1*0.001*i));
    double maxIn = 0.0;
    double maxOut = 0.0;
    double maxBetween = 0.0;
    double input;
    for (i = 1000000; i < 1100000; i++) {
        input = sin(0.1*0.001*i);
        ikNotchList_step(&list, input);
        output = ikNotchList_getOutput(&list, 0);
        maxIn = fmax(maxIn, fabs(input));
        maxOut = fmax(maxOut, fabs(output));
        output = ikNotchList_getOutput(&list, 1);
        maxBetween = fmax(maxBetween, fabs(output));
    }
    if (0.001 < maxOut) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikNotchList_test) message=expected zero amplitude at output, but got amplitude of %f\n", maxOut);
    if (0.001 < fabs(maxBetween - maxIn)) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikNotchList_test) message=expected unit amplitude at intermediate signal, but got amplitude of %f\n", maxBetween);
    /* and see that indices out of bounds get saturated */
    if (ikNotchList_getOutput(&list, 0) != ikNotchList_getOutput(&list, -1)) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikNotchList_test) message=expected getOutput to return the same with indices 0 and -1, but it did not\n");
    if (ikNotchList_getOutput(&list, 1) != ikNotchList_getOutput(&list, 2*IKNOTCHLIST_NMAX)) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikNotchList_test) message=expected getOutput to return the same with indices 1 and %d, but it did not\n", 2*IKNOTCHLIST_NMAX);
    ;
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikNotchList_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testInitErrors (ikNotchList_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 testInitErrors (ikNotchList_test) \n");

    printf("%%TEST_STARTED%% testNormal (ikNotchList_test)\n");
    testNormal();
    printf("%%TEST_FINISHED%% time=0 testNormal (ikNotchList_test) \n");

    printf("%%TEST_STARTED%% testBadFrequency (ikNotchList_test)\n");
    testBadFrequency();
    printf("%%TEST_FINISHED%% time=0 testBadFrequency (ikNotchList_test) \n");

    printf("%%TEST_STARTED%% testGetOutput (ikNotchList_test)\n");
    testGetOutput();
    printf("%%TEST_FINISHED%% time=0 testGetOutput (ikNotchList_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
