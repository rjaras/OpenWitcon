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
 * @file ikVfnotch_test.c
 * 
 * @brief Class ikVfnotch unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ikVfnotch.h"

/*
 * Simple C Test Suite for class ikVfnotch
 */

/**
 * Utility: runs a number of cycles of a sine input signal through a given
 * instance of ikVfnotch and checks the last cycle to see that the attenuation
 * is as expected at a given frequency.
 * @param testname name of the test calling this function
 * @param notch instance of ikVfnotch to be used
 * @param dT sampling time
 * @param freq frequency at which to check attenuation
 * @param att expected attenuation
 * @param ncycles number of cycles to run through the filter
 */
void verifyAttenuation(const char *testname, ikVfnotch *notch, double dT, double freq, double att, double ncycles) {
    /*run the filter for 10/dampDen cycles of a sine signal */
    double PI = acos(-1);
    int i = 0;
    int n = (int) ceil(ncycles/dT/freq*2*PI);
    double *output = malloc((n+1)*sizeof(double));
    double aux;
    while (1) {
        output[i] = ikVfnotch_step(notch, sin(i*freq*dT));
        aux = ikVfnotch_getOutput(notch);
        if (fabs(aux-output[i]) > 1e-9) {
            printf("%%TEST_FAILED%% time=0 testname=%s (ikVfnotch_test) message=i == %d, getOutput expected to return %f, but returned %f\n",testname, i, output[i], aux);
            i = n;
            break;
        }
        if (n < i+1) break;
        i++;
    }
    
    /*check the last cycle */
    int j = 0;
    double minval = 0;
    double maxval = 0;
    while (1) {
        minval = fmin(minval, output[i-j]);
        maxval = fmax(maxval, output[i-j]);
        if (1 < (j++)*dT*freq/2/PI) break;
    }
    if (fabs(1 + maxval/minval) > 0.01) printf("%%TEST_FAILED%% time=0 testname=%s (ikVfnotch_test) message=more than 1%% difference between minimum and maximum output values\n", testname);
    if (fabs(att - maxval) > 0.01) printf("%%TEST_FAILED%% time=0 testname=%s (ikVfnotch_test) message=more than 1%% error in attenuation\n", testname);

    /*free resources */
    free(output);
}

/**
 * Utility: runs a few cycles of a sine signal through a given instance of
 * ikVfnotch, then checks that the minimum and maximum output values during the
 * last cycle are within 1% of -1 times each other, and that the maximum value
 * during the last cycle is within 1% of the expected attenuation.
 * Three different frequencies are used for the input sine signal: 0.1, 1 and
 * 10 times the filter notch frequency.
 * @param testname name of the test calling this function
 * @param notch instance of ikVfnotch to be used
 * @param dT samplint time
 * @param freq filter frequency
 * @param dampDen filter denominator damping coefficient
 * @param dampNum filter numerator damping coefficient
 */
void verifyNotchFiltering(const char* testname, ikVfnotch *notch, double dT, double freq, double dampDen, double dampNum) {
    
    verifyAttenuation(testname, notch, dT, freq, dampNum/dampDen, 10/dampDen);
    verifyAttenuation(testname, notch, dT, 0.1*freq, 1.0, 10/dampDen);
    verifyAttenuation(testname, notch, dT, 10*freq, 1.0, 10/dampDen);
    
}

/**
 * Check that instances do what they are supposed to according to arguments
 * passed to init.
 */
void testInit() {
    printf("ikVfnotch_test init\n");
    /*declare instance */
    ikVfnotch notch;
    
    /*initialise instance */
    int err = ikVfnotch_init(&notch, 0.01, 1.0, 0.5, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=init (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("init", &notch, 0.01, 1.0, 0.5, 0.05);
    
    /*initialise instance */
    err = ikVfnotch_init(&notch, 0.007, 0.3, 0.2, 0.04);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=init (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("init", &notch, 0.007, 0.3, 0.2, 0.04);
}

/**
 * Checks that attenuation is as expected after the sampling time is changed
 * via setSamplingTime. It also checks that getSamplingTime returns the relevant
 * value.
 */
void testSamplingTime() {
    printf("ikVfnotch_test sampling_time\n");
    /*declare instance */
    ikVfnotch notch;
    
    /*initialise instance */
    int err = ikVfnotch_init(&notch, 0.01, 1.0, 0.5, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=sampling_time (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("sampling_time", &notch, 0.01, 1.0, 0.5, 0.05);
    double dT = ikVfnotch_getSamplingTime(&notch);
    if (fabs(0.01-dT) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=sampling_time (ikVfnotch_test) message=getSamplingTime expected to return 0.01, but returned %f\n", dT);
    
    /*change the sampling time */
    err = ikVfnotch_setSamplingTime(&notch, 0.002);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=sampling_time (ikVfnotch_test) message=setSamplingTime expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("sampling_time", &notch, 0.002, 1.0, 0.5, 0.05);
    dT = ikVfnotch_getSamplingTime(&notch);
    if (fabs(0.002-dT) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=sampling_time (ikVfnotch_test) message=getSamplingTime expected to return 0.002, but returned %f\n", dT);
    
    /*change the sampling time again */
    err = ikVfnotch_setSamplingTime(&notch, 0.005);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=sampling_time (ikVfnotch_test) message=setSamplingTime expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("sampling_time", &notch, 0.005, 1.0, 0.5, 0.05);
    dT = ikVfnotch_getSamplingTime(&notch);
    if (fabs(0.005-dT) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=sampling_time (ikVfnotch_test) message=getSamplingTime expected to return 0.005, but returned %f\n", dT);
    
}

/**
 * Checks that attenuation is as expected after the filter frequency is changed
 * via setFreq. It also checks that getFreq returns the relevant value.
 */
void testFrequency() {
    printf("ikVfnotch_test frequency\n");
    /*declare instance */
    ikVfnotch notch;
    
    /*initialise instance */
    int err = ikVfnotch_init(&notch, 0.01, 1.0, 0.5, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=frequency (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("frequency", &notch, 0.01, 1.0, 0.5, 0.05);
    double freq = ikVfnotch_getFreq(&notch);
    if (fabs(1.0-freq) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=frequency (ikVfnotch_test) message=getFreq expected to return 1.0, but returned %f\n", freq);
    
    /*change the frequency */
    err = ikVfnotch_setFreq(&notch, 0.5);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=frequency (ikVfnotch_test) message=setFreq expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("frequency", &notch, 0.01, 0.5, 0.5, 0.05);
    freq = ikVfnotch_getFreq(&notch);
    if (fabs(0.5-freq) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=frequency (ikVfnotch_test) message=getFreq expected to return 0.5, but returned %f\n", freq);
    
    /*change the frequency again */
    err = ikVfnotch_setFreq(&notch, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=frequency (ikVfnotch_test) message=setFreq expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("frequency", &notch, 0.01, 0.05, 0.5, 0.05);
    freq = ikVfnotch_getFreq(&notch);
    if (fabs(0.05-freq) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=frequency (ikVfnotch_test) message=getFreq expected to return 0.05, but returned %f\n", freq);
    
}

/**
 * Checks that attenuation is as expected after the filter damping coefficients
 * are changed via setDamp. It also checks that getDamp returns the relevant
 * values.
 */
void testDamping() {
    printf("ikVfnotch_test damping\n");
    /*declare instance */
    ikVfnotch notch;
    
    /*initialise instance */
    int err = ikVfnotch_init(&notch, 0.01, 1.0, 0.5, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("damping", &notch, 0.01, 1.0, 0.5, 0.05);
    double dampDen, dampNum;
    ikVfnotch_getDamp(&notch, &dampDen, &dampNum);
    if (fabs(0.5-dampDen) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=dampDen expected to be 0.5, but it is %f\n", dampDen);
    if (fabs(0.05-dampNum) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=dampNum expected to be 0.05, but it is %f\n", dampNum);
    
    /*change damping */
    ikVfnotch_setDamp(&notch, 0.4, 0.2);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=setDamp expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("damping", &notch, 0.01, 1.0, 0.4, 0.2);
    ikVfnotch_getDamp(&notch, &dampDen, &dampNum);
    if (fabs(0.4-dampDen) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=dampDen expected to be 0.4, but it is %f\n", dampDen);
    if (fabs(0.2-dampNum) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=dampNum expected to be 0.2, but it is %f\n", dampNum);
    
    /*change damping again */
    ikVfnotch_setDamp(&notch, 0.6, 0.014);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=setDamp expected to return 0, but returned %d\n", err);
    /*check it does what it should */
    verifyNotchFiltering("damping", &notch, 0.01, 1.0, 0.6, 0.014);
    ikVfnotch_getDamp(&notch, &dampDen, &dampNum);
    if (fabs(0.6-dampDen) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=dampDen expected to be 0.6, but it is %f\n", dampDen);
    if (fabs(0.014-dampNum) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=damping (ikVfnotch_test) message=dampNum expected to be 0.014, but it is %f\n", dampNum);
    
}

/**
 * Check that init returns the right error codes
 */
void testInitErrors() {
    printf("ikVfnotch_test init_errors\n");
    /*declare instance */
    ikVfnotch notch;
    
    /*0: no error */
    int err = ikVfnotch_init(&notch, 0.01, 1.0, 0.5, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=init_errors (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
    
    /*-1: invalid sampling time, must be positive */
    err = ikVfnotch_init(&notch, 0.0, 1.0, 0.5, 0.05);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=init_errors (ikVfnotch_test) message=init expected to return -1, but returned %d\n", err);
    err = ikVfnotch_init(&notch, -0.01, 1.0, 0.5, 0.05);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=init_errors (ikVfnotch_test) message=init expected to return -1, but returned %d\n", err);
    
    /*-2: invalid frequency, must be positive */
    err = ikVfnotch_init(&notch, 0.01, 0.0, 0.5, 0.05);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=init_errors (ikVfnotch_test) message=init expected to return -2, but returned %d\n", err);
    err = ikVfnotch_init(&notch, 0.01, -1.0, 0.5, 0.05);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=init_errors (ikVfnotch_test) message=init expected to return -2, but returned %d\n", err);
    
}

/**
 * Check that setSamplingTime returns the right error codes
 */
void testSetSamplingTimeErrors() {
    printf("ikVfnotch_test setSamplingTime_errors\n");
    /*declare instance */
    ikVfnotch notch;
    
    /*initialise instance */
    int err = ikVfnotch_init(&notch, 0.01, 1.0, 0.5, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=setSamplingTime_errors (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
        
    /*0: no error */
    err = ikVfnotch_setSamplingTime(&notch, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=setSamplingTime_errors (ikVfnotch_test) message=setSamplingTime expected to return 0, but returned %d\n", err);
    
    /*-1: invalid sampling time, must be positive */
    err = ikVfnotch_setSamplingTime(&notch, 0.0);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=setSamplingTime_errors (ikVfnotch_test) message=setSamplingTime expected to return -1, but returned %d\n", err);
    err = ikVfnotch_setSamplingTime(&notch, -0.01);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=setSamplingTime_errors (ikVfnotch_test) message=setSamplingTime expected to return -1, but returned %d\n", err);
    
}

/**
 * Check that setSamplingTime returns the right error codes
 */
void testSetFrequencyErrors() {
    printf("ikVfnotch_test setFrequency_errors\n");
    /*declare instance */
    ikVfnotch notch;
    
    /*initialise instance */
    int err = ikVfnotch_init(&notch, 0.01, 1.0, 0.5, 0.05);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=setFrequency_errors (ikVfnotch_test) message=init expected to return 0, but returned %d\n", err);
        
    /*0: no error */
    err = ikVfnotch_setSamplingTime(&notch, 2.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=setFrequency_errors (ikVfnotch_test) message=setFreq expected to return 0, but returned %d\n", err);
    
    /*-2: invalid frequency, must be positive */
    err = ikVfnotch_setFreq(&notch, 0.0);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=setFrequency_errors (ikVfnotch_test) message=setFreq expected to return -2, but returned %d\n", err);
    err = ikVfnotch_setFreq(&notch, -2.0);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=setFrequency_errors (ikVfnotch_test) message=setFreq expected to return -2, but returned %d\n", err);
    
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikVfnotch_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% init (ikVfnotch_test)\n");
    testInit();
    printf("%%TEST_FINISHED%% time=0 init (ikVfnotch_test) \n");

    printf("%%TEST_STARTED%% sampling_time (ikVfnotch_test)\n");
    testSamplingTime();
    printf("%%TEST_FINISHED%% time=0 sampling_time (ikVfnotch_test) \n");

    printf("%%TEST_STARTED%% frequency (ikVfnotch_test)\n");
    testFrequency();
    printf("%%TEST_FINISHED%% time=0 frequency (ikVfnotch_test) \n");

    printf("%%TEST_STARTED%% damping (ikVfnotch_test)\n");
    testDamping();
    printf("%%TEST_FINISHED%% time=0 damping (ikVfnotch_test) \n");

    printf("%%TEST_STARTED%% init_errors (ikVfnotch_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 init_errors (ikVfnotch_test) \n");

    printf("%%TEST_STARTED%% setSamplingTime_errors (ikVfnotch_test)\n");
    testSetSamplingTimeErrors();
    printf("%%TEST_FINISHED%% time=0 setSamplingTime_errors (ikVfnotch_test) \n");

    printf("%%TEST_STARTED%% setFrequency_errors (ikVfnotch_test)\n");
    testSetFrequencyErrors();
    printf("%%TEST_FINISHED%% time=0 setFrequency_errors (ikVfnotch_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
