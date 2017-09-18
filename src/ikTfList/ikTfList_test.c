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
 * @file ikTfList_test.c
 * 
 * @brief Class ikTfList unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../ikTfList/ikTfList.h"

/*
 * Simple C Test Suite
 */

/**
 * Pass bad arguments in a few different TFs and see that init returns the right
 * error codes, and that the corresponding TFs saturate but are otherwise unit
 * gains.
 */
void testInitErrors() {
    printf("ikTfList_test testInitErrors\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikTfList list;
    /* declare initialisation parameters */
    ikTfListParams params;
    
    /* set a[0] to zero on the third transfer function, and enable it */
    ikTfList_initParams(&params);
    params.tfParams[2].enable = 1;
    params.tfParams[2].a[0] = 0.0;
    err = ikTfList_init(&list, &params);
    /* see that initialisation failed with error code -3 */
    if (-3 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected error code -3, but got %d\n", err);
    /* see that the third transfer function is a unit gain */
    output = ikTfList_step(&list, 32.0);
    if (fabs(32.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected output to be 32.0, but got %f\n", output);
    output = ikTfList_getOutput(&list, 2);
    if (fabs(32.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected intermediate output to be 32.0, but got %f\n", output);
    
    /* set a[0] to zero on the second transfer function, set it to saturate in */
    /* all directions, and enable it */
    double maxInput = 4.0;
    double minInput = -2.0;
    ikTfList_initParams(&params);
    params.tfParams[1].enable = 1;
    params.tfParams[1].a[0] = 0.0;
    params.tfParams[1].minInput = &minInput;
    params.tfParams[1].maxInput = &maxInput;
    err = ikTfList_init(&list, &params);
    /* see that initialisation failed with error code -2 */
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected error code -2, but got %d\n", err);
    /* see that the second transfer function is a unit gain within the saturation limits */
    output = ikTfList_step(&list, 1.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected output to be 1.0, but got %f\n", output);
    output = ikTfList_step(&list, -1.0);
    if (fabs(-1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected output to be -1.0, but got %f\n", output);
    /* see that the second transfer function is saturating */
    output = ikTfList_step(&list, 16.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected output to be 4.0, but got %f\n", output);
    output = ikTfList_getOutput(&list, 2);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected intermediate output to be 16.0, but got %f\n", output);
    output = ikTfList_step(&list, -16.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected output to be -2.0, but got %f\n", output);
    output = ikTfList_getOutput(&list, 2);
    if (fabs(-16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikTfList_test) message=expected intermediate output to be -16.0, but got %f\n", output);
    
}

/**
 * See that the TFs are applied in the right order, that they can be disabled on
 * the run, and that the saturation works properly. Static gains are used, because
 * proper TF implementation is tested in ikSlti_test.c.
 */
void testNormal() {
    printf("ikTfList_test testNormal\n");
    /* declare error code */
    int err;
    /* declare output; */
    double output;
    /* declare instance */
    ikTfList list;
    /* declare init params */
    ikTfListParams params;
    
    /* see that, by default, one gets a static unit gain */
    ikTfList_initParams(&params);
    err = ikTfList_init(&list, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=init expected to return 0, but returned %d\n", err);
    output = ikTfList_step(&list, 16.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return 16.0, but returned %f\n", output);
    output = ikTfList_step(&list, -16.0);
    if (fabs(-16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return -16.0, but returned %f\n", output);
    
    /* see that transfer function 1 can be a gain of 2.0 */
    ikTfList_initParams(&params);
    params.tfParams[0].enable = 1;
    params.tfParams[0].b[0] = 2.0;
    err = ikTfList_init(&list, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=init expected to return 0, but returned %d\n", err);
    output = ikTfList_step(&list, 2.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return 4.0, but returned %f\n", output);
    output = ikTfList_getOutput(&list, 1);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=intermediate value expected to be 2.0, but is %f\n", output);
    output = ikTfList_step(&list, -2.0);
    if (fabs(-4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return -4.0, but returned %f\n", output);
    output = ikTfList_getOutput(&list, 1);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=intermediate value expected to be -2.0, but is %f\n", output);
    
    /* see that transfer functions are applied in series */
    ikTfList_initParams(&params);
    params.tfParams[0].enable = 1;
    params.tfParams[0].b[0] = 2.0;
    params.tfParams[1].enable = 1;
    params.tfParams[1].a[0] = 0.5;
    err = ikTfList_init(&list, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=init expected to return 0, but returned %d\n", err);
    output = ikTfList_step(&list, 2.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return 8.0, but returned %f\n", output);
    
    /* see that online enabling and saturation works */
    int enableOut = 1;
    int enableIn = 1;
    double maxOutput = 64.0;
    double minOutput = -64.0;
    double maxInput = 0.5;
    double minInput = -0.5;
    ikTfList_initParams(&params);
    params.tfParams[0].variableEnable = &enableOut;
    params.tfParams[1].variableEnable = &enableIn;
    params.tfParams[1].b[0] = 2.0;
    params.tfParams[0].maxOutput = &maxOutput;
    params.tfParams[0].minOutput = &minOutput;
    params.tfParams[1].maxOutput = &maxInput;
    params.tfParams[1].minOutput = &minInput;
    err = ikTfList_init(&list, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=init expected to return 0, but returned %d\n", err);
    output = ikTfList_step(&list, 0.3);
    if (fabs(0.5-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return 0.5, but returned %f\n", output);
    output = ikTfList_step(&list, -0.3);
    if (fabs(-0.5-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return -0.5, but returned %f\n", output);
    maxInput = 0.025;
    output = ikTfList_step(&list, 0.3);
    if (fabs(0.025-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return 0.025, but returned %f\n", output);
    minInput = -2.0;
    output = ikTfList_step(&list, -0.5);
    if (fabs(-1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return -1.0, but returned %f\n", output);
    enableIn = 0;
    output = ikTfList_step(&list, 100.0);
    if (fabs(64.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return 64.0, but returned %f\n", output);
    output = ikTfList_step(&list, -100.0);
    if (fabs(-64.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=step expected to return -64.0, but returned %f\n", output);
    
    /* see that a negaive index gets the last output */
    ikTfList_initParams(&params);
    params.tfParams[0].enable = 1;
    params.tfParams[0].b[0] = 2.0;
    params.tfParams[1].enable = 1;
    params.tfParams[1].a[0] = 0.5;
    err = ikTfList_init(&list, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=init expected to return 0, but returned %d\n", err);
    ikTfList_step(&list, 1.0);
    output = ikTfList_getOutput(&list, -1);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=getOutput expected to return 4.0, but returned %f\n", output);
    output = ikTfList_getOutput(&list, 2*IKTFLIST_NMAX);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testNormal (ikTfList_test) message=getOutput expected to return 1.0, but returned %f\n", output);
    
}

/**
 * See that, when saturation limits are wrong, i.e. the maximum is smaller than
 * the minimum, they are ignored and effective saturation limits remain at their
 * correct values.
 */
void testBadSaturation() {
    printf("ikTfList_test testBadSaturation\n");
    /* declare error code */
    int err;
    /* declare output; */
    double output;
    /* declare instance */
    ikTfList list;
    /* declare init params */
    ikTfListParams params;
    
    /* see that saturation keeps working at the last reasonable level when unreasonable */
    /* levels are specified, and that responsiveness is recovered when reasonable */
    /* levels are specified again */
    double minOutput = -1.0;
    double maxOutput = 1.0;
    ikTfList_initParams(&params);
    params.tfParams[0].enable = 1;
    params.tfParams[0].minOutput = &minOutput;
    params.tfParams[0].maxOutput = &maxOutput;
    err = ikTfList_init(&list, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testBadSaturation (ikTfList_test) message=init expected to return 0, but returned %d\n", err);
    output = ikTfList_step(&list, 2.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testBadSaturation (ikTfList_test) message=step expected to return 1.0, but returned %f\n", output);
    minOutput = 5.0;
    maxOutput = 4.0;
    output = ikTfList_step(&list, 10.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testBadSaturation (ikTfList_test) message=step expected to return 1.0, but returned %f\n", output);
    minOutput = 4.0;
    output = ikTfList_step(&list, 10.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testBadSaturation (ikTfList_test) message=step expected to return 4.0, but returned %f\n", output);
    
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikTfList_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testInitErrors (ikTfList_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 testInitErrors (ikTfList_test) \n");

    printf("%%TEST_STARTED%% testNormal (ikTfList_test)\n");
    testNormal();
    printf("%%TEST_FINISHED%% time=0 testNormal (ikTfList_test) \n");

    printf("%%TEST_STARTED%% testBadSaturation (ikTfList_test)\n");
    testBadSaturation();
    printf("%%TEST_FINISHED%% time=0 testBadSaturation (ikTfList_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
