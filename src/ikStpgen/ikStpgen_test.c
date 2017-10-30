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
 * @file ikStpgen_test.c
 * 
 * @brief Class ikStpgen unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../ikStpgen/ikStpgen.h"

/*
 * Simple C Test Suite
 */

/**
 * Init returns the right error codes when passed bad intialisation parameters
 */
void testInitErrors() {
    printf("ikStpgen_test testInitErrors\n");
    /* declare error code */
    int err;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    
    /* -3 for invalid hysteresis values */
    ikStpgen_initParams(&params);
    params.nzones = 2;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 3.0;
    params.setpoints[1][1] = 4.0;
    params.zoneTransitionHysteresis[0] = -0.1;
    err = ikStpgen_init(&sg, &params);
    if (-3 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -3, but it returned %d\n", err);
    ikStpgen_initParams(&params);

    /* -4 for invalid zone number */
    ikStpgen_initParams(&params);
    params.nzones = -1;
    err = ikStpgen_init(&sg, &params);
    if (-4 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -4, but it returned %d\n", err);
    ikStpgen_initParams(&params);
    params.nzones = IKSTPGEN_NZONEMAX + 1;
    err = ikStpgen_init(&sg, &params);
    if (-4 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -4, but it returned %d\n", err);

    /* -5 for unsorted setpoints */
    ikStpgen_initParams(&params);
    params.nzones = 2;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 1.5;
    params.setpoints[1][1] = 3.0;
    err = ikStpgen_init(&sg, &params);
    if (-5 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -5, but it returned %d\n", err);
    ikStpgen_initParams(&params);
    params.nzones = 2;
    params.setpoints[0][0] = 2.0;
    params.setpoints[1][0] = 1.5;
    params.setpoints[0][1] = 2.5;
    params.setpoints[1][1] = 3.0;
    err = ikStpgen_init(&sg, &params);
    if (-5 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -5, but it returned %d\n", err);

    /* -6 for negative zone transition step number */
    ikStpgen_initParams(&params);
    params.nzones = 3;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.setpoints[0][2] = 16.0;
    params.setpoints[1][2] = 32.0;
    params.nZoneTransitionSteps[0] = -1;
    params.nZoneTransitionSteps[1] = 1;
    err = ikStpgen_init(&sg, &params);
    if (-6 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -6, but it returned %d\n", err);
    ikStpgen_initParams(&params);
    params.nzones = 3;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.setpoints[0][2] = 16.0;
    params.setpoints[1][2] = 32.0;
    params.nZoneTransitionSteps[0] = 1;
    params.nZoneTransitionSteps[1] = -1;
    err = ikStpgen_init(&sg, &params);
    if (-6 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -6, but it returned %d\n", err);

    /* -7 for negative zone transition lock step number */
    ikStpgen_initParams(&params);
    params.nzones = 3;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.setpoints[0][2] = 16.0;
    params.setpoints[1][2] = 32.0;
    params.nZoneTransitionSteps[0] = 1;
    params.nZoneTransitionSteps[1] = 1;
    params.nZoneTransitionLockSteps[0] = -1;
    params.nZoneTransitionLockSteps[1] = 1;
    err = ikStpgen_init(&sg, &params);
    if (-7 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -7, but it returned %d\n", err);
    ikStpgen_initParams(&params);
    params.nzones = 3;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.setpoints[0][2] = 16.0;
    params.setpoints[1][2] = 32.0;
    params.nZoneTransitionSteps[0] = 1;
    params.nZoneTransitionSteps[1] = 1;
    params.nZoneTransitionLockSteps[0] = 1;
    params.nZoneTransitionLockSteps[1] = -1;
    err = ikStpgen_init(&sg, &params);
    if (-7 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -7, but it returned %d\n", err);

    /* -8 for negative control action limit rate */
    ikStpgen_initParams(&params);
    params.controlActionLimitRate = -1.0;
    err = ikStpgen_init(&sg, &params);
    if (-8 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -8, but it returned %d\n", err);

    /* -9 for non-boolean zone transition pre-lock value */
    ikStpgen_initParams(&params);
    params.zoneTransitionPrelock = 2.0;
    err = ikStpgen_init(&sg, &params);
    if (-9 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikStpgen_test) message=expected init to return -9, but it returned %d\n", err);

}

/**
 * Initialisation with default parameters results in the expected behaviour
 */
void testDefault() {
    printf("ikStpgen_test testDefault\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;

    /* see that, by default, we get the inputs waved through unmodified to the outputs, */
    /* and that the preferred control action is 0.0 */
    ikStpgen_initParams(&params);
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);
    /* setpoint gets waved through */
    output = ikStpgen_step(&sg, 1.0, 4.0, 8.0, 0.5, 2.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=step expected to return 1.0, but it returned %f\n", output);
    output = 0.0;
    err = ikStpgen_getOutput(&sg, &output, "setpoint");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=expected getOutput to return 0 for setpoint, but it returned %d\n", err);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=getOutput expected to fetch 1.0 for setpoint, but it fetched %f\n", output);
    /* minimum control action gets waved through */
    output = 0.0;
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=expected getOutput to return 0 for minimum control action, but it returned %d\n", err);
    if (fabs(0.5-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=getOutput expected to fetch 0.5 for minimum control action, but it fetched %f\n", output);
    /* maximum control action gets waved through */
    output = 0.0;
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=expected getOutput to return 0 for maximum control action, but it returned %d\n", err);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=getOutput expected to fetch 2.0 for maximum control action, but it fetched %f\n", output);
    /* preferred control action curve is a unit gain */
    output = 0.0;
    err = ikStpgen_getOutput(&sg, &output, "preferred control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=expected getOutput to return 0 for preferred control action, but it returned %d\n", err);
    if (fabs(0.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=getOutput expected to fetch 0.0 for preferred control action, but it fetched %f\n", output);

    /* see that, by default, the open loop gain is set to negative */
    ikStpgen_initParams(&params);
    params.nzones = 1;
    params.setpoints[0][0] = 0.0;
    params.setpoints[1][0] = 2.0;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);
    ikStpgen_step(&sg, 10.0, 4.0, 8.0, -64.0, 64.0);
    /* so, the preferred control action becomes the minimum control action at the output */
    output = 0.0;
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=expected getOutput to return 0 for minimum control action, but it returned %d\n", err);
    if (fabs(0.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikStpgen_test) message=getOutput expected to fetch 0.0 for minimum control action, but it fetched %f\n", output);

}

/**
 * State transitions from states numbered 4i + 0 happen as expected.
 * This state corresponds to the setpoint being at the lower end of the i-th
 * zone where the preferred control action is applicable.
 */
void testStateMachineTransitionsFrom0() {
    printf("ikStpgen_test testStateMachineTransitionsFrom0\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca; /* here, arbitrarily, we'll manually make pca equal the feedback within the zones, and saturate at the setpoints*/

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 2;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.zoneTransitionHysteresis[0] = 2.0;
    params.preferredControlAction = &pca;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* stay at state 0, so setpoint at 1.0 and maximum control action at preferred control action for speed 1.0 */
    pca = 1.0; /* because at speed 0.0, we are left of the first zone */
    output = ikStpgen_step(&sg, 128.0, 0.0, 0.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 1.0, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=getOutput expected to fetch 1.0 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but it fetched %f\n", output);

    /* move from state 0 to state 2, so setpoint at 2.0 and minimum control action at preferred control action  */
    pca = 1.75; /* inside the first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 128.0, 1.75, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (0.001 < fabs(output - 1.75)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=getOutput expected to fetch 1.75 for minimum control action, but it fetched %f\n", output);
    /* reinitialise the instance to go back to state 0 */
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* move from state 0 to state 1, so setpoint at external maximum setpoint and minimum control action at preferred control action */
    pca = 1.3; /* inside the first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 1.5, 1.3, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 1.5, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (0.001 < fabs(output - 1.3)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=getOutput expected to fetch 1.3 for minimum control action, but it fetched %f\n", output);
    /* reinitialise the instance to go back to state 0 */
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* move from state 4, so setpoint at 4.0 and maximum control action at preferred control action, */
    /* to state 2, so setpoint at 2.0 and minimum control action at preferred control action. */
    /* Cause it with the external maximum setpoint */
    pca = 8.0; /* saturated at last setpoint */
    output = ikStpgen_step(&sg, 128.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 128.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    pca = 3.0; /*saturated at maximum setpoint */
    output = ikStpgen_step(&sg, 3.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    /* reinitialise the instance to go back to state 0 */
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* move from state 4, so setpoint at 4.0 and maximum control action at preferred control action, */
    /* to state 2, so setpoint at 2.0 and minimum control action at preferred control action */
    pca = 8.0; /* saturated at last setpoint */
    output = ikStpgen_step(&sg, 128.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 128.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    pca = 4.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 128.0, 4.0, 1.5, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    /* reinitialise the instance to go back to state 0 */
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* stay at state 4, so setpoint at 4.0 and maximum control action at preferred control action */
    pca = 8.0; /* saturated at last setpoint */
    output = ikStpgen_step(&sg, 128.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 128.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    pca = 4.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 128.0, 4.0, 3.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    /* reinitialise the instance to go back to state 0 */
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom0 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

}

/**
 * State transitions from states numbered 4i + 1 happen as expected.
 * This state corresponds to the setpoint being at the external maximum setpoint.
 */
void testStateMachineTransitionsFrom1() {
    printf("ikStpgen_test testStateMachineTransitionsFrom1\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca; /* here, arbitrarily, we'll manually make pca equal the feedback within the zones, and saturate at the setpoints*/

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 2;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.zoneTransitionHysteresis[0] = 2.0;
    params.preferredControlAction = &pca;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* stay at state 1, so setpoint at external maximum setpoint and minimum control action at preferred control action */
    pca = 1.3; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 1.5, 1.3, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=expected step to return 1.5, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (0.001 < fabs(output - 1.3)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=getOutput expected to fetch 1.3 for minimum control action, but it fetched %f\n", output);
    pca = 1.4; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 1.6, 1.4, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.6)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=expected step to return 1.6, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (0.001 < fabs(output - 1.4)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=getOutput expected to fetch 1.4 for minimum control action, but it fetched %f\n", output);

    /* move from state 1 to state 2 and back */
    pca = 1.3; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 128.0, 1.3, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=step expected to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 1.5, 1.3, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=step expected to return 1.5, but it returned %f\n", output);

    /* move from state 1 to state 0 and back, using the external maximum setpoint */
    pca = 0.8; /* saturated at maximum setpoint */
    output = ikStpgen_step(&sg, 0.8, 1.3, 0.0, -256.0, 256.0);
    if (fabs(0.8-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=step expected to return 0.8, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but it fetched %f\n", output);
    pca = 1.3; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 1.5, 1.3, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=step expected to return 1.5, but it returned %f\n", output);

    /* move from state 1 to state 0 and back, using the feedback */
    pca = 1.1; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 1.5, 1.1, 0.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=step expected to return 1.0, but it returned %f\n", output);
    pca = 1.3; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 1.5, 1.3, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom1 (ikStpgen_test) message=step expected to return 1.5, but it returned %f\n", output);

}

/**
 * State transitions from states numbered 4i + 2 happen as expected.
 * This state corresponds to the setpoint being at the lower end of the i-th
 * zone where the preferred control action is applicable.
 */
void testStateMachineTransitionsFrom2() {
    printf("ikStpgen_test testStateMachineTransitionsFrom2\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 2;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* stay at state 2 */
    output = ikStpgen_step(&sg, 128.0, 128.0, 128.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 3.0, 1.75, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* move from state 2 to state 1 and back */
    output = ikStpgen_step(&sg, 1.5, 1.75, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 1.5, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 2.5, 1.75, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* move from state 2 to state 0 and back */
    output = ikStpgen_step(&sg, 2.5, 1.2, 0.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 1.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 2.5, 1.7, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* stay at state 2 no matter how high the control action, because maxSp is too low */
    output = ikStpgen_step(&sg, 3.0, 1.7, 5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    
    /* move from state 2 to state 4 */
    output = ikStpgen_step(&sg, 128.0, 1.7, 5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

    /* stay at state 6 no matter how high the control action */
    output = ikStpgen_step(&sg, 128.0, 7.0, 5.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 128.0, 9.0, 256.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testStateMachineTransitionsFrom2 (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);

}

/**
 * The control action limits are according to the open loop gain sign.
 */
void testOpenLoopGainSign() {
    printf("ikStpgen_test testOpenLoopGainSign\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca;

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.openLoopGainSign = 75; /* 1 would make more sense, but it should tolerate other numbers, only the sign matters */
    params.nzones = 2;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.preferredControlAction = &pca;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* state 0, open loop gain sign positive, so minimum control action at preferred control action */
    pca = -1;
    output = ikStpgen_step(&sg, 128.0, 0.0, 0.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected step to return 1.0, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(-1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch -1.0 for minimum control action, but it fetched %f\n", output);

    /* state 1, open loop gain sign positive, so maximum control action at preferred control action */
    pca = -1.3;
    output = ikStpgen_step(&sg, 1.5, 1.3, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected step to return 1.5, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (0.001 < fabs(output + 1.3)) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch -1.3 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but it fetched %f\n", output);

    /* state 1, open loop gain sign positive, so maximum control action at preferred control action (but saturated) */
    pca = -1.5;
    output = ikStpgen_step(&sg, 1.5, 1.7, 0.0, -256.0, 256.0);
    if (0.001 < fabs(output - 1.5)) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected step to return 1.5, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (0.001 < fabs(output + 1.5)) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch -1.5 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but it fetched %f\n", output);

    /* state 2, open loop gain sign positive, so maximum control action at preferred control action */
    pca = -1.7;
    output = ikStpgen_step(&sg, 2.5, 1.7, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (0.001 < fabs(output + 1.7)) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch -1.7 for maximum control action, but it fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected getOutput to return 0, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but it fetched %f\n", output);

    /* move from state 2 to state 4, open loop gain sign positive, so with a small */
    /* enough control action */
    pca = -1.7;
    output = ikStpgen_step(&sg, 16.0, 1.7, -5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testOpenLoopGainSign (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

}

/**
 * All named signals are accessible and correctly fetched.
 */
void testGetOutput() {
    printf("ikStpgen_test testGetOutput\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca = 128.0;

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.preferredControlAction = &pca;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);
    output = ikStpgen_step(&sg, -512.0, -64.0, 32.0, -256.0, 256.0);

    /* feedback */
    err = ikStpgen_getOutput(&sg, &output, "feedback");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for feedback, but it returned %d\n", err);
    if (fabs(-64.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch -64.0 for feedback, but it fetched %f\n", output);

    /* control action */
    err = ikStpgen_getOutput(&sg, &output, "control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for control action, but it returned %d\n", err);
    if (fabs(32.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch 32.0 for control action, but it fetched %f\n", output);

    /* preferred control action */
    err = ikStpgen_getOutput(&sg, &output, "preferred control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for preferred control action, but it returned %d\n", err);
    if (fabs(128.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch 128.0 for preferred control action, but it fetched %f\n", output);

    /* external maximum control action */
    err = ikStpgen_getOutput(&sg, &output, "external maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for external maximum control action, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch 256.0 for external maximum control action, but it fetched %f\n", output);

    /* external minimum control action */
    err = ikStpgen_getOutput(&sg, &output, "external minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for external minimum control action, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch -256.0 for external minimum control action, but it fetched %f\n", output);

    /* external maximum setpoint */
    err = ikStpgen_getOutput(&sg, &output, "external maximum setpoint");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for external maximum setpoint, but it returned %d\n", err);
    if (fabs(-512.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch -512.0 for external maximum setpoint, but it fetched %f\n", output);

    /* maximum control action */
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for maximum control action, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch 256.0 for maximum control action, but it fetched %f\n", output);

    /* minimum control action */
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for minimum control action, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch -256.0 for minimum control action, but it fetched %f\n", output);

    /* setpoint */
    err = ikStpgen_getOutput(&sg, &output, "setpoint");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return 0 for setpoint, but it returned %d\n", err);
    if (fabs(-512.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to fetch -512.0 for setpoint, but it fetched %f\n", output);

}

/**
 * Invalid signal names result in the right error codes.
 */
void testGetOutputErrors() {
    printf("ikStpgen_test testGetOutputErrors\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca;

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.preferredControlAction = &pca;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);
    output = ikStpgen_step(&sg, -512.0, -64.0, 32.0, -256.0, 256.0);

    /* -1 for ffeedback */
    err = ikStpgen_getOutput(&sg, &output, "ffeedback");
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikStpgen_test) message=expected getOutput to return -1 for ffeedback, but it returned %d\n", err);

}

/**
 * When transitioning between zones, the setpoint changes at the specified rate
 * and transitions are locked for the specified period.
 */
void testSmoothZoneTransitions() {
    printf("ikStpgen_test testSmoothZoneTransitions\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca; /* here, arbitrarily, we'll manually make pca equal the feedback within the zones, and saturate at the setpoints*/

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 3;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.setpoints[0][1] = 4.0;
    params.setpoints[1][1] = 8.0;
    params.setpoints[0][2] = 16.0;
    params.setpoints[1][2] = 32.0;
    params.zoneTransitionHysteresis[0] = 2.0;
    params.zoneTransitionHysteresis[1] = 8.0;
    params.preferredControlAction = &pca;
    params.nZoneTransitionSteps[0] = 2;
    params.nZoneTransitionSteps[1] = 4;
    params.nZoneTransitionLockSteps[0] = 2;
    params.nZoneTransitionLockSteps[1] = 3;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* get setpoint 1.0 */
    pca = 1.0; /* saturated at smallest setpoint */
    output = ikStpgen_step(&sg, 64.0, 0.0, 0.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 1.0, but it returned %f\n", output);

    /* get setpoint 2.0 immediately */
    pca = 2.0; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 2.0, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* stay at 2.0 until two steps have passed with a large control action */
    pca = 2.0; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 2.0, 5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, 1.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, 5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, 1.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, 5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, 5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    
    /* get setpoint 4.0 in two extra steps, despite very low control action after the zone transition */
    pca = 2.0; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 2.0, 5.0, -256.0, 256.0);
    if (fabs(3.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 3.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, -5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

    /* get setpoint 4.0 twice more after that, despite very low control action after the zone transition */
    pca = 2.0; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 2.0, -5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, -5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

    /* get setpoint 8.0 immediately */
    pca = 7.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 7.0, 5.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);

    /* stay at 8.0 until three steps have passed with a large control action */
    pca = 7.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 7.0, 20.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, 20.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, 20.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    
    /* get setpoint 16.0 in four extra steps, despite very low control action after the zone transition */
    pca = 7.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 7.0, 20.0, -256.0, 256.0);
    if (fabs(10.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 10.0, but it returned %f\n", output);
    pca = 16.0; /* saturated at lower end of third zone */
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(12.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 12.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(14.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 14.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);

    /* get setpoint 16.0 thrice more after that, despite very low control action after the zone transition */
    pca = 16.0; /* saturated at lower end of third zone */
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);

    /* get setpoint 8.0 in four extra steps, despite very high control action after the zone transition */
    pca = 16.0; /* saturated at lower setpoint of third zone */
    output = ikStpgen_step(&sg, 64.0, 9.0, 7.0, -256.0, 256.0);
    if (fabs(14.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 14.0, but it returned %f\n", output);
    pca = 8.0; /* saturated at upper end of second zone */
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(12.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 12.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(10.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 10.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);

    /* get setpoint 8.0 thrice more after that, despite very high control action after the zone transition */
    pca = 8.0; /* saturated at upper setpoint of second zone */
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);

    /* get setpoint 4.0 immediately */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 7.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

    /* stay at 4.0 until two steps have passed with a small control action */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 1.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 5.0, 1.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    
    /* get setpoint 2.0 in two extra steps, despite very high control action after the zone transition */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 1.0, -256.0, 256.0);
    if (fabs(3.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 3.0, but it returned %f\n", output);
    pca = 2.0; /* saturated at upper end of first zone */
    output = ikStpgen_step(&sg, 64.0, 5.0, 11.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* get setpoint 2.0 twice more after that, despite very high control action after the zone transition */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 11.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 5.0, 11.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* get setpoint 1.0 immediately */
    pca = 1.2; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 1.2, 1.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 1.0, but it returned %f\n", output);

    /* reinitialise the instance with zone transition pre-lock disabled */
    params.zoneTransitionPrelock = 0;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected init to return 0, but it returned %d\n", err);

    /* get setpoint 1.0 */
    pca = 1.0; /* saturated at smallest setpoint */
    output = ikStpgen_step(&sg, 64.0, 0.0, 0.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 1.0, but it returned %f\n", output);

    /* get setpoint 2.0 immediately */
    pca = 2.0; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 2.0, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* get setpoint 4.0 in two extra steps, despite very low control action after the zone transition */
    pca = 2.0; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 2.0, 5.0, -256.0, 256.0);
    if (fabs(3.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 3.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, -5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

    /* get setpoint 4.0 twice more after that, despite very low control action after the zone transition */
    pca = 2.0; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 2.0, -5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 2.0, -5.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

    /* get setpoint 8.0 immediately */
    pca = 7.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 7.0, 5.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);

    /* get setpoint 16.0 in four extra steps, despite very low control action after the zone transition */
    pca = 7.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 7.0, 20.0, -256.0, 256.0);
    if (fabs(10.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 10.0, but it returned %f\n", output);
    pca = 16.0; /* saturated at lower end of third zone */
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(12.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 12.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(14.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 14.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);

    /* get setpoint 16.0 thrice more after that, despite very low control action after the zone transition */
    pca = 16.0; /* saturated at lower end of third zone */
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 7.0, -20.0, -256.0, 256.0);
    if (fabs(16.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 16.0, but it returned %f\n", output);

    /* get setpoint 8.0 in four extra steps, despite very high control action after the zone transition */
    pca = 16.0; /* saturated at lower end of third zone */
    output = ikStpgen_step(&sg, 64.0, 9.0, 7.0, -256.0, 256.0);
    if (fabs(14.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 14.0, but it returned %f\n", output);
    pca = 8.0; /* saturated at upper end of second zone */
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(12.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 12.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(10.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 10.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);

    /* get setpoint 8.0 thrice more after that, despite very high control action after the zone transition */
    pca = 8.0; /* saturated at upper end of second zone */
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 9.0, 17.0, -256.0, 256.0);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 8.0, but it returned %f\n", output);

    /* get setpoint 4.0 immediately */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 7.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);

    /* stay at 4.0 until two steps have passed with a small control action */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 1.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 5.0, 1.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 4.0, but it returned %f\n", output);
    
    /* get setpoint 2.0 in two extra steps, despite very high control action after the zone transition */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 1.0, -256.0, 256.0);
    pca = 2.0; /* saturated at upper end of first zone */
    if (fabs(3.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 3.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 5.0, 11.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* get setpoint 2.0 twice more after that, despite very high control action after the zone transition */
    pca = 5.0; /* inside second zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 5.0, 11.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);
    output = ikStpgen_step(&sg, 64.0, 5.0, 11.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 2.0, but it returned %f\n", output);

    /* get setpoint 1.0 immediately */
    pca = 1.2; /* inside first zone, so equal to feedback */
    output = ikStpgen_step(&sg, 64.0, 1.2, 1.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmoothZoneTransitions (ikStpgen_test) message=expected step to return 1.0, but it returned %f\n", output);

}

/**
 * The setpoint is correct when the external maximum setpoint is below all zones
 */
void testSmallExternalMaximumSetpoint() {
    printf("ikStpgen_test testSmallExternalMaximumSetpoint\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 1;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSmallExternalMaximumSetpoint (ikStpgen_test) message=init expected to return 0, but returned %d\n", err);

    /* see that a low external maximum setpoint prevents a transition from 0 to 1 */
    output = ikStpgen_step(&sg, 0.5, 0.4, 0.0, -256.0, 256.0);
    if (fabs(0.5-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmallExternalMaximumSetpoint (ikStpgen_test) message=step expected to return 0.5, but returned %f\n", output);
    output = ikStpgen_step(&sg, 0.5, 1.2, 0.0, -256.0, 256.0);
    if (fabs(0.5-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmallExternalMaximumSetpoint (ikStpgen_test) message=step expected to return 0.5, but returned %f\n", output);
    output = ikStpgen_step(&sg, 2.5, 1.2, 0.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmallExternalMaximumSetpoint (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    
    /* re-initialise instance */
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSmallExternalMaximumSetpoint (ikStpgen_test) message=init expected to return 0, but returned %d\n", err);

    /* see that a low external maximum setpoint prevents a transition from 0 to 2 */
    output = ikStpgen_step(&sg, 0.5, 0.4, 0.0, -256.0, 256.0);
    if (fabs(0.5-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmallExternalMaximumSetpoint (ikStpgen_test) message=step expected to return 0.5, but returned %f\n", output);
    output = ikStpgen_step(&sg, 0.5, 1.8, 0.0, -256.0, 256.0);
    if (fabs(0.5-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSmallExternalMaximumSetpoint (ikStpgen_test) message=step expected to return 0.5, but returned %f\n", output);
    
}

/**
 * The control action limits always allow at least one control action value
 */
void testSensibleControlActionLimits() {
    printf("ikStpgen_test testSensibleControlActionLimits\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 1;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSensibleControlActionLimits (ikStpgen_test) message=init expected to return 0, but returned %d\n", err);

    /* see that external limits are respected no matter what */
    /* here at state 0, with a big minimum control action */
    ikStpgen_step(&sg, 256.0, 1.0, 1.0, 128.0, 256.0);
    ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (fabs(128.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSensibleControlActionLimits (ikStpgen_test) message=step expected to return 128.0, but returned %f\n", output);
    /* here at state 0, with a small maximum control action */
    ikStpgen_step(&sg, 256.0, 1.0, 1.0, -256.0, -128.0);
    ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (fabs(-128.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSensibleControlActionLimits (ikStpgen_test) message=step expected to return -128.0, but returned %f\n", output);
    /* here at state 1, with a big minimum control action */
    ikStpgen_step(&sg, 1.7, 2.0, 1.0, 64.0, 256.0);
    ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (fabs(64.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSensibleControlActionLimits (ikStpgen_test) message=step expected to return 64.0, but returned %f\n", output);
    /* here at state 1, with a small maximum control action */
    ikStpgen_step(&sg, 1.7, 2.0, 1.0, -256.0, -64.0);
    ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (fabs(-64.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSensibleControlActionLimits (ikStpgen_test) message=step expected to return -64.0, but returned %f\n", output);
    /* here at state 2, with a big minimum control action */
    ikStpgen_step(&sg, 256.0, 2.0, 1.0, 32.0, 256.0);
    ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (fabs(32.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSensibleControlActionLimits (ikStpgen_test) message=step expected to return 32.0, but returned %f\n", output);
    /* here at state 2, with a small maximum control action */
    ikStpgen_step(&sg, 256.0, 2.0, 1.0, -256.0, -32.0);
    ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (fabs(-32.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testSensibleControlActionLimits (ikStpgen_test) message=step expected to return -32.0, but returned %f\n", output);

}

void testControlActionLimitRate() {
    printf("ikStpgen_test testControlActionLimitRate\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca; /* here, arbitrarily, we'll manually make pca equal the feedback (or minus it) within the zones, and saturate at the setpoints*/

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 1;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.preferredControlAction = &pca;
    params.controlActionLimitRate = 0.3;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=init expected to return 0, but returned %d\n", err);

    /* OL gain sign negative. State 0. Control action far above preferred control action. */
    pca = 1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, 5.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(5.0 - 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 4.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 0. Control action far below preferred control action. */
    pca = 1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, -5.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(-5.0 + 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -4.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 0. Control action a bit above preferred control action. */
    pca = 1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, 1.2, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(1.1-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 1.1 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 0. Control action a bit below preferred control action. */
    pca = 1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, 1.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(1.1-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 1.1 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 2. Control action far below preferred control action. */
    pca = 1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, -5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-5.0 + 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -4.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 2. Control action far above preferred control action. */
    pca = 1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, 5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(5.0 - 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 4.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 2. Control action a bit below preferred control action. */
    pca = 1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, 1.7, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(1.9-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 1.9 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 2. Control action a bit above preferred control action. */
    pca = 1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, 2.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(1.9-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 1.9 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 1. Control action far below preferred control action. */
    pca = 1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, -4.0, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-4.0 + 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -3.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 1. Control action far above preferred control action. */
    pca = 1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, 4.0, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(4.0 - 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 3.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 1. Control action a bit below preferred control action. */
    pca = 1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, 1.6, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(1.7-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 1.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign negative. State 1. Control action a bit above preferred control action. */
    pca = 1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, 1.8, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(1.7-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 1.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /*make OL gain positive */
    ikStpgen_initParams(&params);
    params.openLoopGainSign = 1.0;
    params.nzones = 1;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.preferredControlAction = &pca;
    params.controlActionLimitRate = 0.3;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=init expected to return 0, but returned %d\n", err);
    
    /* OL gain sign positive. State 0. Control action far below preferred control action. */
    pca = -1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, -5.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-5.0 + 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -4.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 0. Control action far above preferred control action. */
    pca = -1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, 5.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(5.0 - 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 4.7 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 0. Control action a bit below preferred control action. */
    pca = -1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, -1.2, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-1.1-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -1.1 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 0. Control action a bit above preferred control action. */
    pca = -1.1;
    output = ikStpgen_step(&sg, 3.0, 1.1, -1.0, -256.0, 256.0);
    if (fabs(1.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-1.1-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -1.1 for minimum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 256.0 for maximum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 2. Control action far above preferred control action. */
    pca = -1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, 5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(5.0 - 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 4.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 2. Control action far below preferred control action. */
    pca = -1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, -5.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(-5.0 + 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -4.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 2. Control action a bit above preferred control action. */
    pca = -1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, -1.8, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(-1.9-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -1.9 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 2. Control action a bit below preferred control action. */
    pca = -1.9;
    output = ikStpgen_step(&sg, 3.0, 1.9, -2.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 2.0, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(-1.9-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -1.9 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 1. Control action far above preferred control action. */
    pca = -1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, 4.0, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(4.0 - 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch 3.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 1. Control action far below preferred control action. */
    pca = -1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, -4.0, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(-4.0 + 0.3-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -3.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 1. Control action a bit above preferred control action. */
    pca = -1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, -1.6, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(-1.7-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -1.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
    /* OL gain sign positive. State 1. Control action a bit below preferred control action. */
    pca = -1.7;
    output = ikStpgen_step(&sg, 1.75, 1.7, -1.8, -256.0, 256.0);
    if (fabs(1.75-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=step expected to return 1.75, but returned %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (fabs(-1.7-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -1.7 for maximum control action, but fetched %f\n", output);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionLimitRate (ikStpgen_test) message=getOutput expected to fetch -256.0 for minimum control action, but fetched %f\n", output);
    
}

void testExternalLimitCollisionWithPreferred() {
    printf("ikStpgen_test testExternalLimitCollisionWithPreferred\n");
    /* declare error code */
    int err;
    /* declare output */
    double output = 0.0;
    /* declare instance */
    ikStpgen sg;
    /* declare initialisation parameters */
    ikStpgenParams params;
    /* allocate preferred control action*/
    double pca = 2.0; /* here, arbitrarily, we'll leave pca constant here*/

    /* initialise instance */
    ikStpgen_initParams(&params);
    params.nzones = 1;
    params.setpoints[0][0] = 1.0;
    params.setpoints[1][0] = 2.0;
    params.preferredControlAction = &pca;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=init expected to return 0, but returned %d\n", err);

    /* see that external maximum control action is applied although <2.0 */
    ikStpgen_step(&sg, 2.0, 2.0, 0.0, 0.0, 1.0);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (1e-9 < fabs(output - 1.0)) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to fetch 1.0 for minimum control action, but fetched %f\n", output);
    
    /* see that external minimum control action is applied although >2.0 */
    pca = 
    ikStpgen_step(&sg, 2.0, 1.0, 0.0, 3.0, 5.0);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (1e-9 < fabs(output - 3.0)) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to fetch 3.0 for maximum control action, but fetched %f\n", output);
    
    /* re-initialise instance with reverse sign */
    params.openLoopGainSign = 1;
    err = ikStpgen_init(&sg, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=init expected to return 0, but returned %d\n", err);

    /* see that external maximum control action is applied although <2.0 */
    ikStpgen_step(&sg, 2.0, 1.0, 0.0, 0.0, 1.0);
    err = ikStpgen_getOutput(&sg, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to return 0 for minimum control action, but returned %d\n", err);
    if (1e-9 < fabs(output - 1.0)) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to fetch 1.0 for minimum control action, but fetched %f\n", output);
    
    /* see that external minimum control action is applied although >2.0 */
    ikStpgen_step(&sg, 2.0, 2.0, 0.0, 3.0, 5.0);
    err = ikStpgen_getOutput(&sg, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to return 0 for maximum control action, but returned %d\n", err);
    if (1e-9 < fabs(output - 3.0)) printf("%%TEST_FAILED%% time=0 testname=testExternalLimitCollisionWithPreferred (ikStpgen_test) message=getOutput expected to fetch 3.0 for maximum control action, but fetched %f\n", output);
    
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikStpgen_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testInitErrors (ikStpgen_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 testInitErrors (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testDefault (ikStpgen_test)\n");
    testDefault();
    printf("%%TEST_FINISHED%% time=0 testDefault (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testStateMachineTransitionsFrom0 (ikStpgen_test)\n");
    testStateMachineTransitionsFrom0();
    printf("%%TEST_FINISHED%% time=0 testStateMachineTransitionsFrom0 (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testStateMachineTransitionsFrom1 (ikStpgen_test)\n");
    testStateMachineTransitionsFrom1();
    printf("%%TEST_FINISHED%% time=0 testStateMachineTransitionsFrom1 (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testStateMachineTransitionsFrom2 (ikStpgen_test)\n");
    testStateMachineTransitionsFrom2();
    printf("%%TEST_FINISHED%% time=0 testStateMachineTransitionsFrom2 (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testOpenLoopGainSign (ikStpgen_test)\n");
    testOpenLoopGainSign();
    printf("%%TEST_FINISHED%% time=0 testOpenLoopGainSign (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testGetOutput (ikStpgen_test)\n");
    testGetOutput();
    printf("%%TEST_FINISHED%% time=0 testGetOutput (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testGetOutputErrors (ikStpgen_test)\n");
    testGetOutputErrors();
    printf("%%TEST_FINISHED%% time=0 testGetOutputErrors (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testSmoothZoneTransitions (ikStpgen_test)\n");
    testSmoothZoneTransitions();
    printf("%%TEST_FINISHED%% time=0 testSmoothZoneTransitions (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testSmallExternalMaximumSetpoint (ikStpgen_test)\n");
    testSmallExternalMaximumSetpoint();
    printf("%%TEST_FINISHED%% time=0 testSmallExternalMaximumSetpoint (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testSensibleControlActionLimits (ikStpgen_test)\n");
    testSensibleControlActionLimits();
    printf("%%TEST_FINISHED%% time=0 testSensibleControlActionLimits (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testControlActionLimitRate (ikStpgen_test)\n");
    testControlActionLimitRate();
    printf("%%TEST_FINISHED%% time=0 testControlActionLimitRate (ikStpgen_test) \n");

    printf("%%TEST_STARTED%% testExternalLimitCollisionWithPreferred (ikStpgen_test)\n");
    testExternalLimitCollisionWithPreferred();
    printf("%%TEST_FINISHED%% time=0 testExternalLimitCollisionWithPreferred (ikStpgen_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
