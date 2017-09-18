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
 * @file ikSlti_test.c
 * 
 * @brief Class ikSlti unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ikSlti.h"

/*
 * Simple C Test Suite for class ikSlti
 */

/**
 * Test that the constructor returns a well-initialised instance.
 */
void testInit() {
    printf("ikSlti_test init\n");
    /*declare instance */
    ikSlti sys;

    /*initialise instance */
    ikSlti_init(&sys);

    /*see that a == [1 0 0] and b == [1 0 0] */
    double aux1[3];
    double aux2[3];
    ikSlti_getParam(&sys, aux1, aux2);
    if (fabs(1.0-aux1[0]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected a[0]==1.0, but instead a[0]==%f\n", aux1[0]);
    if (fabs(1.0-aux2[0]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected b[0]==1.0, but instead b[0]==%f\n", aux2[0]);
    int i;
    for (i = 1; i < 3; i++) {
        if (fabs(0.0-aux1[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected a[%d]==1.0, but instead a[%d]==%f\n", i, i, aux1[i]);
        if (fabs(0.0-aux2[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected b[%d]==1.0, but instead b[%d]==%f\n", i, i, aux2[i]);
    }

    /*see that inBuff and outBuff are zero */
    ikSlti_getBuff(&sys, aux1, aux2);
    for (i = 1; i < 3; i++) {
        if (fabs(0.0-aux1[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected inBuff[%d]==1.0, but instead inBuff[%d]==%f\n", i, i, aux1[i]);
        if (fabs(0.0-aux2[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected outBuff[%d]==1.0, but instead outBuff[%d]==%f\n", i, i, aux2[i]);
    }

    /*see that saturation limits are set to zeroo and saturation is disabled */
    double min;
    double max;
    int enbl = ikSlti_getInSat(&sys, &min, &max);
    if (0 != enbl) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=getInSat was expected to return 0, but returned %d\n", enbl);
    if (fabs(0.0-min) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected min==0.0, instead min==%f\n", min);
    if (fabs(0.0-max) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected max==0.0, instead max==%f\n", max);
    enbl = ikSlti_getOutSat(&sys, &min, &max);
    if (0 != enbl) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=getOutSat was expected to return 0, but returned %d\n", enbl);
    if (fabs(0.0-min) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected min==0.0, instead min==%f\n", min);
    if (fabs(0.0-max) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected max==0.0, instead max==%f\n", max);
    double output = ikSlti_getOutput(&sys);
    if (fabs(0.0-output) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikSlti_test) message=expected output==0.0, instead output==%f\n", output);

}

/**
 * Set parameters and test that the instance does what it is supposed to do.
 */
void testParameters() {
    printf("ikSlti_test parameters\n");
    /*declare instance */
    ikSlti sys;

    /*initialise instance */
    ikSlti_init(&sys);

    /*see that we have a simple gain of 1 */
    double out = ikSlti_step(&sys, 8.0);
    if (fabs(8.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 8.0, but returned %f\n", out);
    out = ikSlti_getOutput(&sys);
    if (fabs(8.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=output expected to return 8.0, but returned %f\n", out);

    /*see that a gain of 0.25 is successfully got */
    double a[3] = {2.0, 0.0, 0.0};
    double b[3] = {0.5, 0.0, 0.0};
    int err = ikSlti_setParam(&sys, a, b);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=setParam expected to return 0, but returned %d\n", err);
    out = ikSlti_step(&sys, 8.0);
    if (fabs(2.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 2.0, but returned %f\n", out);
    ikSlti_getParam(&sys, a, b);
    if (fabs(2.0-a[0]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected a[0]==2.0, but instead a[0]==%f\n", a[0]);
    if (fabs(0.5-b[0]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected b[0]==0.5, but instead b[0]==%f\n", b[0]);
    int i;
    for (i = 1; i < 3; i++) {
        if (fabs(0.0-a[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected a[%d]==0.0, but instead a[%d]==%f\n", i, i, a[0]);
        if (fabs(0.0-b[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected b[%d]==0.0, but instead b[%d]==%f\n", i, i, b[0]);
    }

    /*re-initialise instance */
    ikSlti_init(&sys);

    /*try some other parameters */
    a[0] = 1.0;
    a[1] = 1.0;
    a[2] = 0.0;
    b[0] = 1.0;
    b[1] = 1.0;
    b[2] = 0.0;
    err = ikSlti_setParam(&sys, a, b);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=setParam expected to return 0, but returned %d\n", err);
    out = ikSlti_step(&sys, 2.0);
    if (fabs(2.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 2.0, but returned %f\n", out);
    out = ikSlti_step(&sys, 2.0);
    if (fabs(2.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 2.0, but returned %f\n", out);
    out = ikSlti_step(&sys, 2.0);
    if (fabs(2.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 2.0, but returned %f\n", out);

    /*re-initialise instance */
    ikSlti_init(&sys);

    /*try some other parameters */
    a[0] = 0.5;
    a[1] = -0.5;
    a[2] = 0.0;
    b[0] = 1.0;
    b[1] = 0.0;
    b[2] = 0.0;
    err = ikSlti_setParam(&sys, a, b);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=setParam expected to return 0, but returned %d\n", err);
    out = ikSlti_step(&sys, 2.0);
    if (fabs(4.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 4.0, but returned %f\n", out);
    out = ikSlti_step(&sys, 2.0);
    if (fabs(8.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 8.0, but returned %f\n", out);
    out = ikSlti_step(&sys, 2.0);
    if (fabs(12.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return 12.0, but returned %f\n", out);
    ikSlti_getParam(&sys, a, b);
    if (fabs(0.5-a[0]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected a[0]==0.5, but instead a[0]==%f\n", a[0]);
    if (fabs(1.0-b[0]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected b[0]==1.0, but instead b[0]==%f\n", b[0]);
    if (fabs(-0.5-a[1]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected a[1]==-0.5, but instead a[1]==%f\n", a[1]);
    if (fabs(0.0-b[1]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=expected b[1]==0.0, but instead b[1]==%f\n", b[1]);

    /*re-initialise instance */
    ikSlti_init(&sys);

    /*try some other parameters */
    a[0] = 1.0;
    a[1] = 1.0;
    a[2] = 1.0;
    b[0] = 1.0;
    b[1] = 2.0;
    b[2] = 4.0;
    err = ikSlti_setParam(&sys, a, b);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=setParam was expected to return 0, but returned %d\n", err);
    out = ikSlti_step(&sys, -2.0);
    if (fabs(-2.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return -2.0, but returned %f\n", out);
    out = ikSlti_step(&sys, -2.0);
    if (fabs(-4.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return -4.0, but returned %f\n", out);
    out = ikSlti_step(&sys, -2.0);
    if (fabs(-8.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=parameters (ikSlti_test) message=step expected to return -8.0, but returned %f\n", out);
}

/**
 * Set buffers and test that the instance does what it is supposed to do.
 */
void testBuffers() {
    printf("ikSlti_test buffers\n");
    /*declare instance */
    ikSlti sys;

    /* initialise instance */
    double a[3], b[3], inBuff[3], outBuff[3];
    a[0] = 1.0;
    a[1] = 0.0;
    a[2] = -1.0;
    b[0] = 0.0;
    b[1] = 0.0;
    b[2] = 1.0;
    inBuff[0] = 2.0;
    inBuff[1] = 4.0;
    inBuff[2] = 8.0;
    outBuff[0] = 16.0;
    outBuff[1] = 32.0;
    outBuff[2] = 64.0;
    ikSlti_init(&sys);
    int err = ikSlti_setParam(&sys, a, b);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=setParam was expected to return 0, but returned %d\n", err);
    ikSlti_setBuff(&sys, inBuff, outBuff);

    /*see that we get the output we expect */
    double out = ikSlti_step(&sys, 0.5);
    if (fabs(36.0 - out) > 1.0E-6) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=step expected to return 36.0, but returned %f\n", out);

    /*see that the buffers are as they should */
    ikSlti_getBuff(&sys, inBuff, outBuff);
    if (fabs(36.0 - outBuff[0]) > 1.0E-6) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=expected outBuff[0]==36.0, but instead outBuff[0]==%f\n", outBuff[0]);
    if (fabs(16.0-outBuff[1]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=expected outBuff[1]==16.0, but instead outBuff[1]==%f\n", outBuff[1]);
    if (fabs(32.0-outBuff[2]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=expected outBuff[2]==32.0, but instead outBuff[2]==%f\n", outBuff[2]);
    if (fabs(0.5-inBuff[0]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=expected inBuff[0]==0.5, but instead inBuff[0]==%f\n", inBuff[0]);
    if (fabs(2.0-inBuff[1]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=expected inBuff[1]==2.0, but instead inBuff[1]==%f\n", inBuff[1]);
    if (fabs(4.0-inBuff[2]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=buffers (ikSlti_test) message=expected inBuff[2]==4.0, but instead inBuff[2]==%f\n", inBuff[2]);


}

/**
 * Set saturation and test that the instance does what it is supposed to do.
 */
void testSaturation() {
    printf("ikSlti_test saturation\n");
    /*declare instatnce */
    ikSlti sys;

    /*initialise instance */
    ikSlti_init(&sys);

    /*see that input saturation works */
    int err = ikSlti_setInSat(&sys, 2, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setInSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    double inBuff[3], outBuff[3];
    ikSlti_getBuff(&sys, inBuff, outBuff);
    int i;
    for (i = 0; i < 3; i++) {
        if (fabs(2.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==2.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(4.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==4.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    
    /*see that just minimum input saturation works */
    err = ikSlti_setInSat(&sys, -1, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setInSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(2.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==2.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(8.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==8.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    
    /*see that just maximum input saturatio works */
    err = ikSlti_setInSat(&sys, 1, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setInSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(1.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==1.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(4.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==4.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    
    /*see that input saturation is properly disabled */
    err = ikSlti_setInSat(&sys, 0, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setInSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(1.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==1.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(8.0-inBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected inBuff[%d]==8.0, but instead inBuff[%d]==%f\n", i, i, inBuff[i]);
    }
    
    /*see that output saturation works */
    err = ikSlti_setOutSat(&sys, 2, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setOutSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(2.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==2.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    double out = ikSlti_getOutput(&sys);
    if (fabs(2.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 2.0, but returned%f\n", out);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(4.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==4.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    out = ikSlti_getOutput(&sys);
    if (fabs(4.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 4.0, but returned%f\n", out);
    
    /*see that just minimum output saturation works */
    err = ikSlti_setOutSat(&sys, -1, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setOutSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(2.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==2.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    out = ikSlti_getOutput(&sys);
    if (fabs(2.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 2.0, but returned%f\n", out);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(8.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==8.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    out = ikSlti_getOutput(&sys);
    if (fabs(8.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 8.0, but returned%f\n", out);
    
    /*see that just maximum output saturation works */
    err = ikSlti_setOutSat(&sys, 1, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setOutSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(1.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==1.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    out = ikSlti_getOutput(&sys);
    if (fabs(1.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 1.0, but returned%f\n", out);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(4.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==4.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    out = ikSlti_getOutput(&sys);
    if (fabs(4.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 4.0, but returned%f\n", out);
    
    /*see that output saturation is disabled properly */
    err = ikSlti_setOutSat(&sys, 0, 2.0, 4.0);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=setOutSat was expected to return 0, but returned %d\n", err);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_step(&sys, 1.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(1.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==1.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    out = ikSlti_getOutput(&sys);
    if (fabs(1.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 1.0, but returned%f\n", out);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_step(&sys, 8.0);
    ikSlti_getBuff(&sys, inBuff, outBuff);
    for (i = 0; i < 3; i++) {
        if (fabs(8.0-outBuff[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=expected outBuff[%d]==8.0, but instead outBuff[%d]==%f\n", i, i, outBuff[i]);
    }
    out = ikSlti_getOutput(&sys);
    if (fabs(8.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=saturation (ikSlti_test) message=getOutput expected to return 8.0, but returned%f\n", out);
    
}

/**
 * Test that method setParam returns the proper errors when passed bad arguments.
 */
void testSetParamErrors() {
    printf("ikSlti_test setParam_errors\n");
    /*declare instance */
    ikSlti sys;
    
    /*see that it returns error code -1 when a[0] != 0 */
    double a[3];
    double b[3];
    a[0] = 0;
    int err = ikSlti_setParam(&sys, a, b);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=setParam_errors (ikSlti_test) message=setParam expected to return -1, but returned %d\n", err);
    
}

/**
 * Test that method setInSat returns the proper errors when passed bad arguments.
 */
void testSetInSatErrors() {
    printf("ikSlti_test setInSat_errors\n");
    /*declare instance */
    ikSlti sys;
    
    /*see that it returns the correct error codes */
    int err = ikSlti_setInSat(&sys, -2, 0.0, 0.0);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=setInSat_errors (ikSlti_test) message=setInSat expected to return -1, but returned %d\n", err);
    err = ikSlti_setInSat(&sys, 2, 1.0, -1.0);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=setInSat_errors (ikSlti_test) message=setInSat expected to return -2, but returned %d\n", err);

}

/**
 * Test that method setOutSat returns the proper errors when passed bad arguments.
 */
void testSetOutSatErrors() {
    printf("ikSlti_test setOutSat_errors\n");
    /*declare instance */
    ikSlti sys;
    
    /*see that it returns the correct error codes */
    int err = ikSlti_setOutSat(&sys, -2, 0.0, 0.0);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=setOutSat_errors (ikSlti_test) message=setOutSat expected to return -1, but returned %d\n", err);
    err = ikSlti_setOutSat(&sys, 2, 1.0, -1.0);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=setOutSat_errors (ikSlti_test) message=setOutSat expected to return -2, but returned %d\n", err);

}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikSlti_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% init (ikSlti_test)\n");
    testInit();
    printf("%%TEST_FINISHED%% time=0 init (ikSlti_test) \n");

    printf("%%TEST_STARTED%% parameters (ikSlti_test)\n");
    testParameters();
    printf("%%TEST_FINISHED%% time=0 parameters (ikSlti_test) \n");

    printf("%%TEST_STARTED%% buffers (ikSlti_test)\n");
    testBuffers();
    printf("%%TEST_FINISHED%% time=0 buffers (ikSlti_test) \n");

    printf("%%TEST_STARTED%% saturation (ikSlti_test)\n");
    testSaturation();
    printf("%%TEST_FINISHED%% time=0 saturation (ikSlti_test) \n");

    printf("%%TEST_STARTED%% setParam_errors (ikSlti_test)\n");
    testSetParamErrors();
    printf("%%TEST_FINISHED%% time=0 setParam_errors (ikSlti_test) \n");

    printf("%%TEST_STARTED%% setInSat_errors (ikSlti_test)\n");
    testSetInSatErrors();
    printf("%%TEST_FINISHED%% time=0 setInSat_errors (ikSlti_test) \n");

    printf("%%TEST_STARTED%% setOutSat_errors (ikSlti_test)\n");
    testSetOutSatErrors();
    printf("%%TEST_FINISHED%% time=0 setOutSat_errors (ikSlti_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
