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
 * @file ikConLoop_test.c
 * 
 * @brief Class ikConLoop unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../ikConLoop/ikConLoop.h"

/*
 * Simple C Test Suite
 */

/**
 * Default initialisation results in the expected behaviour
 */
void testDefault() {
    printf("ikConLoop_test testDefault\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikConLoop loop;
    /* declare initialisation parameters */
    ikConLoopParams params;
    
    /* initialise instance */
    ikConLoop_initParams(&params);
    err = ikConLoop_init(&loop, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikConLoop_test) message=init expected to return 0, but it returned %d\n", err);
    
    /* see that the control action equals the maximum setpoint minus the feedback, */
    /* and that it saturates at the external saturation limits */
    output = ikConLoop_step(&loop, 2.0, 4.0, -256.0, 256.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikConLoop_test) message=step expected to return -2.0, but it returned %f\n", output);
    output = ikConLoop_step(&loop, 2000.0, 4.0, -256.0, 256.0);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikConLoop_test) message=step expected to return 256.0, but it returned %f\n", output);
    output = ikConLoop_step(&loop, -2000.0, 4.0, -256.0, 256.0);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikConLoop_test) message=step expected to return -256.0, but it returned %f\n", output);
    
}

/**
 * Init returns the right error codes when passed bad initialisation parameters
 */
void testInitErrors() {
    printf("ikConLoop_test testInitErrors\n");
    /* declare error code */
    int err;
    /* declare instance */
    ikConLoop loop;
    /* declare initialisation parameters */
    ikConLoopParams params;
    
    /* -1 for bad setpoint generator parameters */
    ikConLoop_initParams(&params);
    params.setpointGenerator.preferredControlActionLutblN = -1;
    err = ikConLoop_init(&loop, &params);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikConLoop_test) message=init expected to return -1, but it returned %d\n", err);
    
    /* -2 for bad linear controller parameters */
    ikConLoop_initParams(&params);
    params.linearController.gainSchedN = -1;
    err = ikConLoop_init(&loop, &params);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikConLoop_test) message=init expected to return -2, but it returned %d\n", err);
    
    /* -3 for bad setpoint filter parameters */
    ikConLoop_initParams(&params);
    params.setpointFilters.demandTfs.tfParams[0].enable = 1;
    params.setpointFilters.demandTfs.tfParams[0].a[0] = 0.0;
    err = ikConLoop_init(&loop, &params);
    if (-3 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikConLoop_test) message=init expected to return -3, but it returned %d\n", err);
    
    /* -4 for bad control action filter parameters */
    ikConLoop_initParams(&params);
    params.controlActionFilters.demandTfs.tfParams[0].enable = 1;
    params.controlActionFilters.demandTfs.tfParams[0].a[0] = 0.0;
    err = ikConLoop_init(&loop, &params);
    if (-4 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikConLoop_test) message=init expected to return -4, but it returned %d\n", err);
    
    /* -5 for bad preset selector parameters */
    ikConLoop_initParams(&params);
    params.regionSelector.nRegions = -1;
    err = ikConLoop_init(&loop, &params);
    if (-5 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikConLoop_test) message=init expected to return -5, but it returned %d\n", err);
    
}

/**
 * The control action is fed back from the linear controller to the setpoint generator
 */
void testControlActionFeedback() {
    printf("ikConLoop_test testControlActionFeedback\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikConLoop loop;
    /* declare initialisation parameters */
    ikConLoopParams params;
    
    /* initialise instance */
    ikConLoop_initParams(&params);
    params.setpointGenerator.nzones = 2;
    params.setpointGenerator.setpoints[0][0] = 0.0;
    params.setpointGenerator.setpoints[1][0] = 1.0;
    params.setpointGenerator.setpoints[0][1] = 2.0;
    params.setpointGenerator.setpoints[1][1] = 4.0;
    params.linearController.errorTfs.tfParams[0].enable = 1;
    params.linearController.errorTfs.tfParams[0].b[0] = -1;
    err = ikConLoop_init(&loop, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testControlActionFeedback (ikConLoop_test) message=init expected to return 0, but it returned %d\n", err);
    
    /* see that the setpoint generator makes it to the second zone, for which */
    /* adequate control action feedback is needed */
    output = ikConLoop_step(&loop, 8.0, 4.0, -256.0, 256.0);
    if (fabs(3.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionFeedback (ikConLoop_test) message=step expected to return 3.0, but it returned %f\n", output);
    output = ikConLoop_step(&loop, 8.0, 4.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testControlActionFeedback (ikConLoop_test) message=step expected to return 2.0, but it returned %f\n", output);
    
}

/**
 * Linear controller can be customised
 */
void testLinearController() {
    printf("ikConLoop_test testLinearController\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikConLoop loop;
    /* declare initialisation parameters */
    ikConLoopParams params;
    
    /* initialise instance */
    ikConLoop_initParams(&params);
    params.linearController.errorTfs.tfParams[0].enable = 1;
    params.linearController.errorTfs.tfParams[0].b[0] = 64.0;
    err = ikConLoop_init(&loop, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testLinearController (ikConLoop_test) message=init expected to return 0, but it returned %d\n", err);
    
    /* see that the linear controller has gain 64.0 */
    output = ikConLoop_step(&loop, 1.0, 0.0, -256.0, 256.0);
    if (fabs(64.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testLinearController (ikConLoop_test) message=step expected to return 64.0, but it returned %f\n", output);
    
}

/**
 * Signals are accessible via getOutput
 */
void testGetOutput() {
    printf("ikConLoop_test testGetOutput\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikConLoop loop;
    /* declare initialisation parameters */
    ikConLoopParams params;
    
    /* initialise instance */
    ikConLoop_initParams(&params);
    err = ikConLoop_init(&loop, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=init expected to return 0, but it returned %d\n", err);
    
    /* run two steps, so signals propagate well */
    output = ikConLoop_step(&loop, 8.0, 4.0, -256.0, 256.0);
    output = ikConLoop_step(&loop, 8.0, 4.0, -256.0, 256.0);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testLinearController (ikConLoop_test) message=step expected to return 4.0, but it returned %f\n", output);
        
    /* external maximum control action */
    err = ikConLoop_getOutput(&loop, &output, "external maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for external maximum control action, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 256.0 for external maximum control action, but it fetched %f\n", output);
    
    /* external minimum control action */
    err = ikConLoop_getOutput(&loop, &output, "external minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for external minimum control action, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch -256.0 for external minimum control action, but it fetched %f\n", output);
    
    /* maximum setpoint */
    err = ikConLoop_getOutput(&loop, &output, "maximum setpoint");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for maximum setpoint, but it returned %d\n", err);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 8.0 for maximum setpoint, but it fetched %f\n", output);
    
    /* feedback */
    err = ikConLoop_getOutput(&loop, &output, "feedback");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for feedback, but it returned %d\n", err);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 4.0 for feedback, but it fetched %f\n", output);
    
    /* maximum control action */
    err = ikConLoop_getOutput(&loop, &output, "maximum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for maximum control action, but it returned %d\n", err);
    if (fabs(256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 256.0 for maximum control action, but it fetched %f\n", output);
    
    /* minimum control action */
    err = ikConLoop_getOutput(&loop, &output, "minimum control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for minimum control action, but it returned %d\n", err);
    if (fabs(-256.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch -256.0 for minimum control action, but it fetched %f\n", output);
    
    /* setpoint */
    err = ikConLoop_getOutput(&loop, &output, "setpoint");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for setpoint, but it returned %d\n", err);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 8.0 for setpoint, but it fetched %f\n", output);
    
    /* control action */
    err = ikConLoop_getOutput(&loop, &output, "control action");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for control action, but it returned %d\n", err);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 4.0 for control action, but it fetched %f\n", output);
    
    /* setpoint generator>setpoint */
    err = ikConLoop_getOutput(&loop, &output, "setpoint generator>setpoint");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for setpoint generator>setpoint, but it returned %d\n", err);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 8.0 for setpoint generator>setpoint, but it fetched %f\n", output);
    
    /* linear controller>demand */
    err = ikConLoop_getOutput(&loop, &output, "linear controller>demand");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for linear controller>demand, but it returned %d\n", err);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 8.0 for linear controller>demand, but it fetched %f\n", output);
    
    /* y */
    err = ikConLoop_getOutput(&loop, &output, "y");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for y, but it returned %d\n", err);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 4.0 for y, but it fetched %f\n", output);
    
    /* x */
    err = ikConLoop_getOutput(&loop, &output, "x");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for x, but it returned %d\n", err);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 8.0 for x, but it fetched %f\n", output);
    
    /* preset selection */
    err = ikConLoop_getOutput(&loop, &output, "preset selection");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for preset selection, but it returned %d\n", err);
    if (fabs(0.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 0.0 for preset selection, but it fetched %f\n", output);
    
    /* setpoint filters>demand */
    err = ikConLoop_getOutput(&loop, &output, "setpoint filters>demand");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for setpoint filters>demand, but it returned %d\n", err);
    if (fabs(8.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 8.0 for setpoint filters>demand, but it fetched %f\n", output);
    
    /* control action filters>demand */
    err = ikConLoop_getOutput(&loop, &output, "control action filters>demand");
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to return 0 for control action filters>demand, but it returned %d\n", err);
    if (fabs(4.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testGetOutput (ikConLoop_test) message=getOutput expected to fetch 4.0 for control action filters>demand, but it fetched %f\n", output);
    
}

/**
 * getOutput returns the right error codes when passed bad arguments
 */
void testGetOutputErrors() {
    printf("ikConLoop_test testGetOutputErrors\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikConLoop loop;
    /* declare initialisation parameters */
    ikConLoopParams params;
    
    /* initialise instance */
    ikConLoop_initParams(&params);
    err = ikConLoop_init(&loop, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testGetOutputErrors (ikConLoop_test) message=init expected to return 0, but it returned %d\n", err);
    
    /* -1 for settttpoint */
    err = ikConLoop_getOutput(&loop, &output, "settttpoint");
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testGetOutputErrors (ikConLoop_test) message=getOutput expected to return -1 for settttpoint, but it returned %d\n", err);
    
    /* -1 for linear controller>dddemand */
    err = ikConLoop_getOutput(&loop, &output, "linear controller>dddemand");
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testGetOutputErrors (ikConLoop_test) message=getOutput expected to return -1 for linear controller>dddemand, but it returned %d\n", err);
    
    /* -2 for lincon>demand */
    err = ikConLoop_getOutput(&loop, &output, "lincon>demand");
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=testGetOutputErrors (ikConLoop_test) message=getOutput expected to return -2 for lincon>demand, but it returned %d\n", err);
    
}

/**
 * The preset selector works as expected
 */
void testPresetSelection() {
    printf("ikConLoop_test testPresetSelection\n");
    /* declare error code */
    int err;
    /* declare output */
    double output;
    /* declare instance */
    ikConLoop loop;
    /* declare initialisation parameters */
    ikConLoopParams params;
    
    /* initialise instance */
    ikConLoop_initParams(&params);
    /* setpoint filters are a static gain of 2.0 */
    params.setpointFilters.demandTfs.tfParams[0].enable = 1;
    params.setpointFilters.demandTfs.tfParams[0].b[0] = 2.0;
    /* control action filters are a static gain of -2.0 */
    params.controlActionFilters.demandTfs.tfParams[0].enable = 1;
    params.controlActionFilters.demandTfs.tfParams[0].b[0] = -2.0;
    /* one triangular region around point {2.0, -4.0} */
    params.regionSelector.nRegions = 1;
    params.regionSelector.regions[0].nPoints = 3;
    params.regionSelector.regions[0].points[0].x = 1.0;
    params.regionSelector.regions[0].points[0].y = -3.5;
    params.regionSelector.regions[0].points[1].x = 3.0;
    params.regionSelector.regions[0].points[1].y = -3.5;
    params.regionSelector.regions[0].points[2].x = 3.0;
    params.regionSelector.regions[0].points[2].y = -5.0;
    /* two presets, 0 for default, 1 for region 1 */
    /* one with gain 2, the other with gain -2 */
    params.linearController.configN = 2;
    params.linearController.demandTfsEnable[0][1] = 1;
    params.linearController.demandTfsEnable[1][2] = 1;
    params.linearController.demandTfs.tfParams[1].b[0] = 2.0;
    params.linearController.demandTfs.tfParams[2].b[0] = -2.0;
    err = ikConLoop_init(&loop, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testPresetSelection (ikConLoop_test) message=init expected to return 0, but it returned %d\n", err);
    
    /* see that, with a setpoint of 1, we alternative have a control action of 2 and -2 */
    output = ikConLoop_step(&loop, 1.0, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testPresetSelection (ikConLoop_test) message=step expected to return 2.0, but it returned %f\n", output);
    output = ikConLoop_step(&loop, 1.0, 0.0, -256.0, 256.0);
    if (fabs(-2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testPresetSelection (ikConLoop_test) message=step expected to return -2.0, but it returned %f\n", output);
    output = ikConLoop_step(&loop, 1.0, 0.0, -256.0, 256.0);
    if (fabs(2.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testPresetSelection (ikConLoop_test) message=step expected to return 2.0, but it returned %f\n", output);
        
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikConLoop_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testDefault (ikConLoop_test)\n");
    testDefault();
    printf("%%TEST_FINISHED%% time=0 testDefault (ikConLoop_test) \n");

    printf("%%TEST_STARTED%% testInitErrors (ikConLoop_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 testInitErrors (ikConLoop_test) \n");

    printf("%%TEST_STARTED%% testControlActionFeedback (ikConLoop_test)\n");
    testControlActionFeedback();
    printf("%%TEST_FINISHED%% time=0 testControlActionFeedback (ikConLoop_test) \n");

    printf("%%TEST_STARTED%% testLinearController (ikConLoop_test)\n");
    testLinearController();
    printf("%%TEST_FINISHED%% time=0 testLinearController (ikConLoop_test) \n");

    printf("%%TEST_STARTED%% testGetOutput (ikConLoop_test)\n");
    testGetOutput();
    printf("%%TEST_FINISHED%% time=0 testGetOutput (ikConLoop_test) \n");

    printf("%%TEST_STARTED%% testGetOutputErrors (ikConLoop_test)\n");
    testGetOutputErrors();
    printf("%%TEST_FINISHED%% time=0 testGetOutputErrors (ikConLoop_test) \n");

    printf("%%TEST_STARTED%% testPresetSelection (ikConLoop_test)\n");
    testPresetSelection();
    printf("%%TEST_FINISHED%% time=0 testPresetSelection (ikConLoop_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
