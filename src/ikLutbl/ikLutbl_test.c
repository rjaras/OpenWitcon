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
 * @file ikLutbl_test.c
 * 
 * @brief Class ikLutbl unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ikLutbl.h"

/*
 * Simple C Test Suite for class ikLutbl
 */

/**
 * Test initialisation.
 */
void testInit() {
    printf("ikLutbl_test init\n");
    /*declare instance */
    ikLutbl tbl;

    /*initialise instance */
    ikLutbl_init(&tbl);

    /*see that initialisation has been as expected */
    int n = ikLutbl_getPointNumber(&tbl);
    if (1 != n) printf("%%TEST_FAILED%% time=0 testname=init (ikLutbl_test) message=getPointNumber was expected to return 1, but returned %d\n", n);
    double x;
    double y;
    int err = ikLutbl_getPoints(&tbl, 1, &x, &y);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=init (ikLutbl_test) message=getPointNumber was expected to return 0, but returned %d\n", err);
    if (fabs(0.0-x) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikLutbl_test) message=expected x[0]==0.0, but instead x[0]==%f\n", x);
    if (fabs(1.0-y) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=init (ikLutbl_test) message=expected y[0]==1.0, but instead y[0]==%f\n", y);

}

/**
 * Set points and test that the instance behaves as expected.
 */
void testPoints() {
    printf("ikLutbl_test points\n");
    /*declare instance */
    ikLutbl tbl;

    /*initialise instance */
    ikLutbl_init(&tbl);

    /*see that we've got a constant output of 1 */
    double out = ikLutbl_eval(&tbl, -16.0);
    if (fabs(1.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return 1.0, but it returned %f\n", out);

    /*see that we get a gain of -1 */
    double x[5], y[5];
    x[0] = -1.0;
    y[0] = 1.0;
    x[1] = 1.0;
    y[1] = -1.0;
    int err = ikLutbl_setPoints(&tbl, 2, x, y);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=setPoints was expected to return 0, but it returned %d\n", err);
    double aux1[5], aux2[5];
    err = ikLutbl_getPoints(&tbl, 2, aux1, aux2);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=getPoints was expected to return 0, but it returned %d\n", err);
    int i;
    for (i = 0; i < 2; i++) {
        if (fabs(aux1[i]-x[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=expected x[%d] == %f, x[%d] == %f\n", i, x[i], i, aux1[i]);
        if (fabs(aux2[i]-y[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=expected y[%d] == %f, y[%d] == %f\n", i, y[i], i, aux2[i]);
    }
    int n = ikLutbl_getPointNumber(&tbl);
    if (2 != n) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=getPointNumber was expected to return 2, but it returned %d\n", n);
    out = ikLutbl_eval(&tbl, 8.0);
    if (fabs(-8.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return -8.0, but it returned %f\n", out);

    /*see that it can deal with unevenly spaced points */
    x[0] = 0.0;
    x[1] = 0.1;
    x[2] = 1.0;
    x[3] = 10.0;
    x[4] = 100.0;
    y[0] = 0.0;
    y[1] = 0.1;
    y[2] = 1.0;
    y[3] = 10.0;
    y[4] = 100.0;
    err = ikLutbl_setPoints(&tbl, 5, x, y);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=setPoints was expected to return 0, but it returned %d\n", err);
    err = ikLutbl_getPoints(&tbl, 5, aux1, aux2);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=getPoints was expected to return 0, but it returned %d\n", err);
    for (i = 0; i < 5; i++) {
        if (fabs(aux1[i]-x[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=expected x[%d] == %f, x[%d] == %f\n", i, x[i], i, aux1[i]);
        if (fabs(aux2[i]-y[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=expected y[%d] == %f, y[%d] == %f\n", i, y[i], i, aux2[i]);
    }
    n = ikLutbl_getPointNumber(&tbl);
    if (5 != n) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=getPointNumber was expected to return 5, but it returned %d\n", n);
    out = ikLutbl_eval(&tbl, 16.0);
    if (fabs(16.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return 16.0, but it returned %f\n", out);
    out = ikLutbl_eval(&tbl, -16.0);
    if (fabs(-16.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return -16.0, but it returned %f\n", out);
    out = ikLutbl_eval(&tbl, 256.0);
    if (fabs(256.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return 256.0, but it returned %f\n", out);
    out = ikLutbl_eval(&tbl, -256.0);
    if (fabs(-256.0-out) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return -256.0, but it returned %f\n", out);

    /* see that it can deal with nonlinear tables */
    x[0] = 0.0;
    x[1] = 0.1;
    x[2] = 1.0;
    x[3] = 10.0;
    x[4] = 100.0;
    y[0] = 0.0;
    y[1] = 0.1;
    y[2] = -1.0;
    y[3] = 10.0;
    y[4] = -100.0;
    err = ikLutbl_setPoints(&tbl, 5, x, y);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=setPoints was expected to return 0, but it returned %d\n", err);
    err = ikLutbl_getPoints(&tbl, 5, aux1, aux2);
    if (0 != err) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=getPoints was expected to return 0, but it returned %d\n", err);
    for (i = 0; i < 5; i++) {
        if (fabs(aux1[i]-x[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=expected x[%d] == %f, x[%d] == %f\n", i, x[i], i, aux1[i]);
        if (fabs(aux2[i]-y[i]) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=expected y[%d] == %f, y[%d] == %f\n", i, y[i], i, aux2[i]);
    }
    n = ikLutbl_getPointNumber(&tbl);
    if (5 != n) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=getPointNumber was expected to return 5, but it returned %d\n", n);
    out = ikLutbl_eval(&tbl, 0.05);
    if (fabs(0.05 - out) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return 0.05, but it returned %f\n", out);
    out = ikLutbl_eval(&tbl, 0.5);
    if (fabs(0.1-1.1*0.4/0.9 - out) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return -0.3888repeat, but it returned %f\n", out);
    out = ikLutbl_eval(&tbl, 5.0);
    if (fabs(-1.0+11.0*4.0/9.0 - out) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return 3.888repeat, but it returned %f\n", out);
    out = ikLutbl_eval(&tbl, 50.0);
    if (fabs(10.0-110.0*40.0/90.0 - out) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=points (ikLutbl_test) message=eval was expected to return -38.888repeat, but it returned %f\n", out);
    
}

/**
 * Test that method getPoints returns the proper errors when passed bad arguments.
 */
void testGetPointsErrors() {
    printf("ikLutbl_test getPoints_errors\n");
    /*declare instance */
    ikLutbl tbl;

    /*initialise instance */
    ikLutbl_init(&tbl);

    /*check that asking for too many points results in an error */
    double x[5], y[5];
    int err = ikLutbl_getPoints(&tbl, 5, x, y);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=getPoints_errors (ikLutbl_test) message=getPoints was expected to return -1, but instead it returned %d\n", err);

}

/**
 * Test that method setPoints returns the proper errors when passed bad arguments.
 */
void testSetPointsErrors() {
    printf("ikLutbl_test setPoints_errors\n");
    /*declare instance */
    ikLutbl tbl;

    /*initialise instance */
    ikLutbl_init(&tbl);

    /*see that trying to set too many or too few points retults in error -1 */
    double *aux1 = malloc((IKLUTBL_MAXPOINTS + 1) * sizeof (double));
    double *aux2 = malloc((IKLUTBL_MAXPOINTS + 1) * sizeof (double));
    int i;
    for (i = 0; i < IKLUTBL_MAXPOINTS + 1; i++) {
        aux1[i] = (double) i;
        aux2[i] = (double) i;
    }
    int err = ikLutbl_setPoints(&tbl, IKLUTBL_MAXPOINTS + 1, aux1, aux2);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=getPoints_errors (ikLutbl_test) message=setPoints was expected to return -1, but instead it returned %d\n", err);
    err = ikLutbl_setPoints(&tbl, 0, aux1, aux2);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=getPoints_errors (ikLutbl_test) message=setPoints was expected to return -1, but instead it returned %d\n", err);

    /*see that trying to set an unsorted array of keys resutls in error -2 */
    aux1[1] = -1.0;
    err = ikLutbl_setPoints(&tbl, 4, aux1, aux2);
    if (-2 != err) printf("%%TEST_FAILED%% time=0 testname=getPoints_errors (ikLutbl_test) message=setPoints was expected to return -2, but instead it returned %d\n", err);

}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikLutbl_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% init (ikLutbl_test)\n");
    testInit();
    printf("%%TEST_FINISHED%% time=0 init (ikLutbl_test) \n");

    printf("%%TEST_STARTED%% points (ikLutbl_test)\n");
    testPoints();
    printf("%%TEST_FINISHED%% time=0 points (ikLutbl_test) \n");

    printf("%%TEST_STARTED%% getPoints_errors (ikLutbl_test)\n");
    testGetPointsErrors();
    printf("%%TEST_FINISHED%% time=0 getPoints_errors (ikLutbl_test) \n");

    printf("%%TEST_STARTED%% setPoints_errors (ikLutbl_test)\n");
    testSetPointsErrors();
    printf("%%TEST_FINISHED%% time=0 setPoints_errors (ikLutbl_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
