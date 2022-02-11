/*
Copyright (C) 2020-2021 IKERLAN

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
 * @file ikSurf_test.c
 *
 * @brief Class ikSurf unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ikSurf.h"

/*
 * Simple C Test Suite for class ikSurf
 */

/**
 * Test one-dimensional surface, i.e. a point.
 */
void testOneDim() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test one D\n");

    /*no way to force an error*/
    printf("new\n");
    dims = 1;
    ndata = NULL;
    data = (double*) malloc(sizeof(double));
    data[0] = -24.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    err = NULL;

    /*eval to datum for dim=0, to 0.0 otherwise*/
    printf("eval\n");
    x = NULL;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-24.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=eval was expected to return -24.0 for dimension 0, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=eval was expected to return 0.0 for dimension 1, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);

    /*getDimensions=1*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (1 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=getDimensions was expected to return 1, but returned %d\n", getDimensionsRet);

    /*getPointNumber=1 for dim 0, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=getPointNumber was expected to return 1 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=one D (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test two-dimensional surface, i.e. a line, with a single point.
 */
void testTwoDimOnePoint() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D one point\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 1;
    data = (double*) malloc(sizeof(double)*2);
    data[0] = 0.3;
    data[1] = 63.4;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D one point (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval to 0-th coordinate to 0.3 and 1-th coordinate to 63.4, all else to 0.0*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 2.1;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(0.3 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D one point(ikSurf_test) message=eval was expected to return 0.3 for dimension 0, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(63.4 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D one point(ikSurf_test) message=eval was expected to return 63.4 for dimension 1, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D one point(ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D one point(ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D one point(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=1 for dim 0 and dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D one point (ikSurf_test) message=getPointNumber was expected to return 1 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D one point (ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D one point(ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D one point(ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test two-dimensional surface, i.e. a line, with two points.
 */
void testTwoDimTwoPoint() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D two point\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 2;
    data = (double*) malloc(sizeof(double)*4);
    data[0] = -10.0;
    data[1] = 10.0;
    data[2] = 20.0;
    data[3] = -20.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D two point (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 0-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = -20.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(10.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return 10.0 for dimension 0 at -20.0, but returned %f\n", evalRet);
    x[0] = 20.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-10.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return -10.0 for dimension 0 at 20.0, but returned %f\n", evalRet);
    x[0] = 60.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-30.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return -30.0 for dimension 0 at 60.0, but returned %f\n", evalRet);
    x[0] = -60.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(30.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return 30.0 for dimension 0 at -60.0, but returned %f\n", evalRet);
    x[0] = 10.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-5.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return -5.0 for dimension 0 at 10.0, but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 10.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-20.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return -20.0 for dimension 1 at 10.0, but returned %f\n", evalRet);
    x[0] = -10.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(20.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return 20.0 for dimension 1 at -10.0, but returned %f\n", evalRet);
    x[0] = 30.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-60.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return -60.0 for dimension 1 at 30.0, but returned %f\n", evalRet);
    x[0] = -30.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(60.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return 60.0 for dimension 1 at -30.0, but returned %f\n", evalRet);
    x[0] = 5.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-10.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return -10.0 for dimension 1 at 5.0, but returned %f\n", evalRet);
    /*eval out-of-range dims to 0.0*/
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (2 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D two point (ikSurf_test) message=getPointNumber was expected to return 2 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D two point(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D two point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D two point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test two-dimensional surface, i.e. a line, with three points.
 */
void testTwoDimThreePoint() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D three point\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 3;
    data = (double*) malloc(sizeof(double)*6);
    data[0] = 7.0;
    data[1] = 8.0;
    data[2] = 9.0;
    data[3] = 1.0;
    data[4] = 3.0;
    data[5] = 9.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D three point (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 0-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 1.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(7.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 7.0 for dimension 0 at 1.0, but returned %f\n", evalRet);
    x[0] = 3.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(8.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 8.0 for dimension 0 at 3.0, but returned %f\n", evalRet);
    x[0] = 9.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 9.0 for dimension 0 at 9.0, but returned %f\n", evalRet);
    x[0] = 0.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(6.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 6.5 for dimension 0 at 0.0, but returned %f\n", evalRet);
    x[0] = 15.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(10.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 10.0 for dimension 0 at 15.0, but returned %f\n", evalRet);
    x[0] = 6.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(8.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 8.5 for dimension 0 at 6.0, but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 7.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at 7.0, but returned %f\n", evalRet);
    x[0] = 8.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 3.0 for dimension 1 at 8.0, but returned %f\n", evalRet);
    x[0] = 9.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 9.0 for dimension 1 at 9.0, but returned %f\n", evalRet);
    x[0] = 9.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(12.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 12.0 for dimension 1 at 9.5, but returned %f\n", evalRet);
    x[0] = 5.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return -3.0 for dimension 1 at 5.0, but returned %f\n", evalRet);
    /*eval out-of-range dims to 0.0*/
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (3 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point (ikSurf_test) message=getPointNumber was expected to return 3 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test two-dimensional surface, i.e. a line, with three points, the middle one being the highest.
 */
void testTwoDimThreePointConcave() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D three point concave\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 3;
    data = (double*) malloc(sizeof(double)*6);
    data[0] = 1.0;
    data[1] = 2.0;
    data[2] = 3.0;
    data[3] = 10.0;
    data[4] = 20.0;
    data[5] = 5.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 0-th coordinate above maximum*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 5.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 3.0 for dimension 0 at 5.0, but returned %f\n", evalRet);
    x[0] = 20.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at 20.0, but returned %f\n", evalRet);
    x[0] = 21.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at 21.0, but returned %f\n", evalRet);
    x[0] = 10.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(2.0+2.0/3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 2.6666 for dimension 0 at 10.0, but returned %f\n", evalRet);
    x[0] = 0.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(3.0+1.0/3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 3.3333 for dimension 0 at 0.0, but returned %f\n", evalRet);
    /*eval 0-th coordinate below maximum*/
    x[0] = 10.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at 10.0, but returned %f\n", evalRet);
    x[0] = 20.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at 20.0, but returned %f\n", evalRet);
    x[0] = 25.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at 25.0, but returned %f\n", evalRet);
    x[0] = 5.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 0.5 for dimension 0 at 5.0, but returned %f\n", evalRet);
    x[0] = -10.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return -1.0 for dimension 0 at -10.0, but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 1.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(10.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 10.0 for dimension 1 at 1.0, but returned %f\n", evalRet);
    x[0] = 2.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(20.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 20.0 for dimension 1 at 2.0, but returned %f\n", evalRet);
    x[0] = 3.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(5.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 5.0 for dimension 1 at 3.0, but returned %f\n", evalRet);
    x[0] = 4.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-10.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return -10.0 for dimension 1 at 4.0, but returned %f\n", evalRet);
    x[0] = -2.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-20.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return -2.0 for dimension 1 at -20.0, but returned %f\n", evalRet);
    /*eval out-of-range dims to 0.0*/
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (3 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point concave  (ikSurf_test) message=getPointNumber was expected to return 3 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point concave (ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point concave  (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point concave  (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test two-dimensional surface, i.e. a line, with three points, the middle one being the lowest.
 */
void testTwoDimThreePointConvex() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D three point convex\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 3;
    data = (double*) malloc(sizeof(double)*6);
    data[0] = 7.0;
    data[1] = 9.0;
    data[2] = 11.0;
    data[3] = 4.0;
    data[4] = -6.0;
    data[5] = 1.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 0-th coordinate above minimum*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 1.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(11.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 11.0 for dimension 0 at 1.0, but returned %f\n", evalRet);
    x[0] = -6.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 9.0 for dimension 0 at -6.0, but returned %f\n", evalRet);
    x[0] = -7.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 9.0 for dimension 0 at -7.0, but returned %f\n", evalRet);
    x[0] = -2.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(10.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 10.0 for dimension 0 at -2.5, but returned %f\n", evalRet);
    x[0] = 4.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(12.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 12.0 for dimension 0 at 4.5, but returned %f\n", evalRet);
    /*eval 0-th coordinate below minimum*/
    x[0] = 4.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(7.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 7.0 for dimension 0 at 4.0, but returned %f\n", evalRet);
    x[0] = -6.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 9.0 for dimension 0 at -6.0, but returned %f\n", evalRet);
    x[0] = -6.1;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 9.0 for dimension 0 at -6.1, but returned %f\n", evalRet);
    x[0] = -1.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(8.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 8.0 for dimension 0 at -1.0, but returned %f\n", evalRet);
    x[0] = 9.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(6.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 6.0 for dimension 0 at 9.0, but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 7.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 4.0 for dimension 1 at 7.0, but returned %f\n", evalRet);
    x[0] = 9.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-6.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return -6.0 for dimension 1 at 9.0, but returned %f\n", evalRet);
    x[0] = 11.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at 11.0, but returned %f\n", evalRet);
    x[0] = 6.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 9.0 for dimension 1 at 6.0, but returned %f\n", evalRet);
    x[0] = 8.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return -1.0 for dimension 1 at 8.0, but returned %f\n", evalRet);
    x[0] = 10.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-2.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return -2.5 for dimension 1 at 10.0, but returned %f\n", evalRet);
    x[0] = 12.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(4.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 4.5 for dimension 1 at 12.0, but returned %f\n", evalRet);
    /*eval out-of-range dims to 0.0*/
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (3 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point convex  (ikSurf_test) message=getPointNumber was expected to return 3 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point convex (ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point convex  (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D three point convex  (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}


/**
 * Test two-dimensional surface, i.e. a line, with four points.
 */
void testTwoDimFourPoint() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D four point\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 4;
    data = (double*) malloc(sizeof(double)*8);
    data[0] = -17.0;
    data[1] = -3.0;
    data[2] = 4.0;
    data[3] = 100.0;
    data[4] = 0.0;
    data[5] = -1.0;
    data[6] = -2.0;
    data[7] = -3.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D four point (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 0-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = -3.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(100.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 100.0 for dimension 0 at -3, but returned %f\n", evalRet);
    x[0] = 0.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-17.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -17.0 for dimension 0 at 0.0, but returned %f\n", evalRet);
    x[0] = -2.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 4.0 for dimension 0 at -2.0, but returned %f\n", evalRet);
    x[0] = -1.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -3.0 for dimension 0 at -1.0, but returned %f\n", evalRet);
    x[0] = -1.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 0.5 for dimension 0 at -1.5, but returned %f\n", evalRet);
    x[0] = 2.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-45.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -45.0 for dimension 0 at 2.0, but returned %f\n", evalRet);
    x[0] = -6.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(388.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 388.0 for dimension 0 at -6.0, but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 196.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -4.0 for dimension 1 at 196.0, but returned %f\n", evalRet);
    x[0] = -24.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 0.5 for dimension 1 at -24.0, but returned %f\n", evalRet);
    x[0] = 52.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-2.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -2.5 for dimension 1 at 52.0, but returned %f\n", evalRet);
    x[0] = -6.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-0.75 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -0.75 for dimension 1 at -6.5, but returned %f\n", evalRet);
    x[0] = -2.3;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-1.1 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -1.1 for dimension 1 at -2.3, but returned %f\n", evalRet);
    x[0] = 196.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return -4.0 for dimension 1 at 196.0, but returned %f\n", evalRet);
    x[0] = -31.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at -31.0, but returned %f\n", evalRet);
    /*eval out-of-range dims to 0.0*/
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point (ikSurf_test) message=getPointNumber was expected to return 4 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test two-dimensional surface, i.e. a line, with four points, the second one being the lowest.
 */
void testTwoDimFourPointConvex() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D four point convex\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 4;
    data = (double*) malloc(sizeof(double)*8);
    data[0] = -100.0;
    data[1] = -50.0;
    data[2] = 50.0;
    data[3] = 100.0;
    data[4] = 3.0;
    data[5] = 2.0;
    data[6] = 4.0;
    data[7] = 5.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D four point convex (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 0-th coordinate linearly above extreme*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 2.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-50.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return -50.0 for dimension 0 at 2.0, but returned %f\n", evalRet);
    x[0] = 4.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(50.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 50.0 for dimension 0 at 4.0, but returned %f\n", evalRet);
    x[0] = 5.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(100.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 100.0 for dimension 0 at 5.0, but returned %f\n", evalRet);
    x[0] = 3.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 0.0 for dimension 0 at 3.0, but returned %f\n", evalRet);
    x[0] = 4.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(75.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 75.0 for dimension 0 at 4.5, but returned %f\n", evalRet);
    x[0] = 6.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(150.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 150.0 for dimension 0 at 6.0, but returned %f\n", evalRet);
    x[0] = 1.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-50.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return -50.0 for dimension 0 at 1.0, but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly below extreme*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 2.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-50.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return -50.0 for dimension 0 at 2.0, but returned %f\n", evalRet);
    x[0] = 3.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-100.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return -100.0 for dimension 0 at 3.0, but returned %f\n", evalRet);
    x[0] = 1.5;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-50.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return -50.0 for dimension 0 at 1.5, but returned %f\n", evalRet);
    x[0] = 4.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-150.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return -150.0 for dimension 0 at 4.0, but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = -100.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 3.0 for dimension 1 at -100.0, but returned %f\n", evalRet);
    x[0] = -50.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 2.0 for dimension 1 at -50.0, but returned %f\n", evalRet);
    x[0] = 50.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 4.0 for dimension 1 at 50.0, but returned %f\n", evalRet);
    x[0] = 100.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(5.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 5.0 for dimension 1 at 100.0, but returned %f\n", evalRet);
    x[0] = -150.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 4.0 for dimension 1 at -150.0, but returned %f\n", evalRet);
    x[0] = -75.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 2.5 for dimension 1 at -75.0, but returned %f\n", evalRet);
    x[0] = 0.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 3.0 for dimension 1 at 0.0, but returned %f\n", evalRet);
    x[0] = 75.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(4.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 4.5 for dimension 1 at 75.0, but returned %f\n", evalRet);
    x[0] = 150.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(6.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 6.0 for dimension 1 at 150.0, but returned %f\n", evalRet);
    /*eval out-of-range dims to 0.0*/
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point convex (ikSurf_test) message=getPointNumber was expected to return 4 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point convex(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point convex (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point convex (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test two-dimensional surface, i.e. a line, with four points, the third one being the highest.
 */
void testTwoDimFourPointConcave() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test two D four point concave\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 4;
    data = (double*) malloc(sizeof(double)*8);
    data[0] = -2.0;
    data[1] = -1.0;
    data[2] = 1.0;
    data[3] = 2.0;
    data[4] = 100.0;
    data[5] = 120.0;
    data[6] = 200.0;
    data[7] = 50.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=two D four point concave (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 0-th coordinate linearly below extreme*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 100.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return -2.0 for dimension 0 at 100.0, but returned %f\n", evalRet);
    x[0] = 120.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return -1.0 for dimension 0 at 120.0, but returned %f\n", evalRet);
    x[0] = 200.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at 200.0, but returned %f\n", evalRet);
    x[0] = 250.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at 250.0, but returned %f\n", evalRet);
    x[0] = 110.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return -1.5 for dimension 0 at 110.0, but returned %f\n", evalRet);
    x[0] = 160.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 0.0 for dimension 0 at 160.0, but returned %f\n", evalRet);
    x[0] = 80.0;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(-3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return -3.0 for dimension 0 at 80.0, but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly above extreme*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 200.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at 200.0, but returned %f\n", evalRet);
    x[0] = 50.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at 50.0, but returned %f\n", evalRet);
    x[0] = 210.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at 210.0, but returned %f\n", evalRet);
    x[0] = -100.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 3.0 for dimension 0 at -100.0, but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 2.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(50.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 50.0 for dimension 1 at 2.0, but returned %f\n", evalRet);
    x[0] = -2.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(100.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 100.0 for dimension 1 at -2.0, but returned %f\n", evalRet);
    x[0] = 1.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(200.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 200.0 for dimension 1 at 1.0, but returned %f\n", evalRet);
    x[0] = -1.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(120.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 120.0 for dimension 1 at -1.0, but returned %f\n", evalRet);
    x[0] = 3.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-100.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return -100.0 for dimension 1 at 3.0, but returned %f\n", evalRet);
    x[0] = -3.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(80.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 80.0 for dimension 1 at -3.0, but returned %f\n", evalRet);
    x[0] = 1.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(125.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 125.0 for dimension 1 at 1.5, but returned %f\n", evalRet);
    x[0] = -1.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(110.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 110.0 for dimension 1 at -1.5, but returned %f\n", evalRet);
    x[0] = 0.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(160.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 160.0 for dimension 1 at 0.0, but returned %f\n", evalRet);
    /*eval out-of-range dims to 0.0*/
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 0.0 for dimension 2, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, -1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=eval was expected to return 0.0 for dimension -1, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point concave (ikSurf_test) message=getPointNumber was expected to return 4 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point concave(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point concave (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=two D four point concave (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test three-dimensional surface, i.e. an actual surface, with four points.
 */
void testThreeDimFourPoint() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test three D four point\n");

    /*won't force an error*/
    printf("new\n");
    dims = 3;
    ndata = (int *) malloc(sizeof(int)*2);
    ndata[0] = 2;
    ndata[1] = 2;
    data = (double*) malloc(sizeof(double)*8);
    data[0] = -1.0;
    data[1] = 1.0;
    data[2] = -2.0;
    data[3] = 2.0;
    data[4] = 2.0;
    data[5] = 1.0;
    data[6] = 3.0;
    data[7] = 4.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=three D four point (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 2-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*2);
    x[0] = -1.0;
    x[1] = -2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.0 for dimension 2 at (-1.0, -2.0), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = -2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 3.0 for dimension 2 at (1.0, -2.0), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = 2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 4.0 for dimension 2 at (1.0, 2.0), but returned %f\n", evalRet);
    x[0] = -1.0;
    x[1] = 2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 1.0 for dimension 2 at (-1.0, 2.0), but returned %f\n", evalRet);
    x[0] = -1.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.75 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 0.75 for dimension 2 at (-1.0, 3.0), but returned %f\n", evalRet);
    x[0] = -2.0;
    x[1] = 2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(-0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return -0.5 for dimension 2 at (-2.0, 2.0), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(4.25 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 4.25 for dimension 2 at (1.0, 3.0), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = 2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(5.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 5.5 for dimension 2 at (2.0, 2.0), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = -2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(3.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 3.5 for dimension 2 at (2.0, -2.0), but returned %f\n", evalRet);
    x[0] = -1.0;
    x[1] = -3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.25 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.25 for dimension 2 at (-1.0, -3.0), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = -3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.75 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.75 for dimension 2 at (1.0, -3.0), but returned %f\n", evalRet);
    x[0] = -2.0;
    x[1] = -2.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 1.5 for dimension 2 at (-2.0, -2.0), but returned %f\n", evalRet);
    x[0] = -2.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(-1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return -1.0 for dimension 2 at (-2.0, 3.0), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(6.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 6.0 for dimension 2 at (2.0, 3.0), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = -3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 3.0 for dimension 2 at (2.0, -3.0), but returned %f\n", evalRet);
    x[0] = -2.0;
    x[1] = -3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.0 for dimension 2 at (-2.0, -3.0), but returned %f\n", evalRet);
    x[0] = 0.0;
    x[1] = 0.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.5 for dimension 2 at (0.0, 0.0), but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 2.0;
    x[1] = 6.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 3.0 for dimension 1 at (2.0, 6.0), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = 5.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.0 for dimension 1 at (2.0, 5.5), but returned %f\n", evalRet);
    x[0] = -1.0;
    x[1] = 1.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.0 for dimension 1 at (-1.0, 1.0), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = 2.75;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return -3.0 for dimension 1 at (1.0, 2.75), but returned %f\n", evalRet);
    x[0] = -2.0;
    x[1] = -1.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 3.0 for dimension 1 at (-2.0, -1.0), but returned %f\n", evalRet);
    x[0] = 0.0;
    x[1] = 2.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 0.0 for dimension 1 at (0.0, 2.5), but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly*/
    x[0] = 2.0;
    x[1] = 4.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at (2.0, 4.0), but returned %f\n", evalRet);
    x[0] = -3.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at (-3.0, 3.0), but returned %f\n", evalRet);
    x[0] = 0.0;
    x[1] = 2.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return 0.0 for dimension 0 at (0.0, 2.5), but returned %f\n", evalRet);
    x[0] = 3.0;
    x[1] = 0.75;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=eval was expected to return -1.0 for dimension 0 at (3.0, 0.75), but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=3*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (3 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=getDimensions was expected to return 3, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=2 dim 1 and getPointNumber=1 for dim 2, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (2 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D four point (ikSurf_test) message=getPointNumber was expected to return 2 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (2 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=getPointNumber was expected to return 2 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D four point(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 3);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D four point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 3, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D four point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test three-dimensional surface, i.e. an actual surface, with sixteen points.
 */
void testThreeDimSixteenPoint() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test three D sixteen point\n");

    /*won't force an error*/
    printf("new\n");
    dims = 3;
    ndata = (int *) malloc(sizeof(int)*2);
    ndata[0] = 4;
    ndata[1] = 4;
    data = (double*) malloc(sizeof(double)*24);
    data[0] = 0.0;
    data[1] = 1.0;
    data[2] = 2.0;
    data[3] = 3.0;
    data[4] = 0.0;
    data[5] = 1.0;
    data[6] = 2.0;
    data[7] = 3.0;
    data[8] = 6.0;
    data[9] = 3.0;
    data[10] = 1.0;
    data[11] = 0.0;
    data[12] = 7.0;
    data[13] = 4.0;
    data[14] = 2.0;
    data[15] = 1.0;
    data[16] = 10.0;
    data[17] = 7.0;
    data[18] = 5.0;
    data[19] = 4.0;
    data[20] = 15.0;
    data[21] = 12.0;
    data[22] = 10.0;
    data[23] = 9.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 2-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*2);
    x[0] = 1.5;
    x[1] = 1.5;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(4.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 4.5 for dimension 2 at (1.5, 1.5), but returned %f\n", evalRet);
    x[0] = -1.0;
    x[1] = -1.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(8.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 8.0 for dimension 2 at (-1.0, -1.0), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(4.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 4.0 for dimension 2 at (2.0, 3.0), but returned %f\n", evalRet);
    x[0] = 0.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 0.0 for dimension 2 at (0.0, 3.0), but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    x[0] = 4.0;
    x[1] = 29.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(-3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return -3.0 for dimension 1 at (4.0, 29.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 4.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 1.5 for dimension 1 at (1.5, 4.5), but returned %f\n", evalRet);
    x[0] = 0.5;
    x[1] = -1.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(4.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 4.5 for dimension 1 at (0.5, -1.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 4.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 1.5 for dimension 1 at (1.5, 4.5), but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly*/
    x[0] = 0.0;
    x[1] = 5.0;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(-1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 4.0 for dimension -1 at (0.0, 5.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 4.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=eval was expected to return 1.5 for dimension 0 at (1.5, 4.5), but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=3*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (3 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=getDimensions was expected to return 3, but returned %d\n", getDimensionsRet);

    /*getPointNumber=4 for dim 0 and getPointNumber=4 dim 1 and getPointNumber=1 for dim 2, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point (ikSurf_test) message=getPointNumber was expected to return 4 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=getPointNumber was expected to return 4 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 3);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 3, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test three-dimensional surface, i.e. an actual surface, with sixteen points, where the central four are higher.
 */
void testThreeDimSixteenPointConcave() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test three D sixteen point concave\n");

    /*won't force an error*/
    printf("new\n");
    dims = 3;
    ndata = (int *) malloc(sizeof(int)*2);
    ndata[0] = 4;
    ndata[1] = 4;
    data = (double*) malloc(sizeof(double)*24);
    data[0] = 0.0;
    data[1] = 1.0;
    data[2] = 2.0;
    data[3] = 3.0;
    data[4] = 0.0;
    data[5] = 1.0;
    data[6] = 2.0;
    data[7] = 3.0;
    data[8] = 1.0;
    data[9] = 1.0;
    data[10] = 1.0;
    data[11] = 1.0;
    data[12] = 1.0;
    data[13] = 2.0;
    data[14] = 2.0;
    data[15] = 1.0;
    data[16] = 1.0;
    data[17] = 2.0;
    data[18] = 2.0;
    data[19] = 1.0;
    data[20] = 1.0;
    data[21] = 1.0;
    data[22] = 1.0;
    data[23] = 1.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 2-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*2);
    x[0] = 1.5;
    x[1] = 1.5;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 2.0 for dimension 2 at (1.5, 1.5), but returned %f\n", evalRet);
    x[0] = -1.0;
    x[1] = -1.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 2.0 for dimension 2 at (-1.0, -1.0), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 2 at (2.0, 3.0), but returned %f\n", evalRet);
    x[0] = 0.0;
    x[1] = 3.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 2 at (0.0, 3.0), but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly above extreme*/
    x[0] = 1.5;
    x[1] = 5.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at (1.5, 5.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 1.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 2.5 for dimension 1 at (1.5, 1.5), but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly below extreme*/
    x[0] = 1.5;
    x[1] = 5.0;
    evalRet = ikSurf_eval(surf, 1, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at (1.5, 5.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 1.5;
    evalRet = ikSurf_eval(surf, 1, x, -1);
    if (fabs(0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 0.5 for dimension 1 at (1.5, 1.5), but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly above extreme*/
    x[0] = 1.5;
    x[1] = 5.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at (1.5, 5.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 1.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 2.5 for dimension 0 at (1.5, 1.5), but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly below extreme*/
    x[0] = 1.5;
    x[1] = 5.0;
    evalRet = ikSurf_eval(surf, 1, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at (1.5, 5.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 1.5;
    evalRet = ikSurf_eval(surf, 1, x, -1);
    if (fabs(0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=eval was expected to return 0.5 for dimension 0 at (1.5, 1.5), but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=3*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (3 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=getDimensions was expected to return 3, but returned %d\n", getDimensionsRet);

    /*getPointNumber=4 for dim 0 and getPointNumber=4 dim 1 and getPointNumber=1 for dim 2, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave (ikSurf_test) message=getPointNumber was expected to return 4 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=getPointNumber was expected to return 4 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 3);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 3, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point concave (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test three-dimensional surface, i.e. an actual surface, with sixteen points, where the central four are lower.
 */
void testThreeDimSixteenPointConvex() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test three D sixteen point convex\n");

    /*won't force an error*/
    printf("new\n");
    dims = 3;
    ndata = (int *) malloc(sizeof(int)*2);
    ndata[0] = 4;
    ndata[1] = 4;
    data = (double*) malloc(sizeof(double)*24);
    data[0] = 0.0;
    data[1] = 1.0;
    data[2] = 2.0;
    data[3] = 3.0;
    data[4] = 0.0;
    data[5] = 1.0;
    data[6] = 2.0;
    data[7] = 3.0;
    data[8] = -1.0;
    data[9] = -2.5;
    data[10] = -3.0;
    data[11] = 0.0;
    data[12] = -2.0;
    data[13] = -5.0;
    data[14] = -4.0;
    data[15] = -1.0;
    data[16] = -3.0;
    data[17] = -4.0;
    data[18] = -3.0;
    data[19] = -2.0;
    data[20] = -2.0;
    data[21] = -3.0;
    data[22] = -2.0;
    data[23] = -1.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 2-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*2);
    x[0] = 2.5;
    x[1] = 0.5;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(-3 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return -3.0 for dimension 2 at (2.5, 0.5), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 2.5;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(-2.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return -2.5 for dimension 2 at (1.5, 2.5), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = 1.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(-5.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return -5.0 for dimension 2 at (1.0, 1.0), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = 4.0;
    evalRet = ikSurf_eval(surf, 2, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 2.0 for dimension 2 at (1.0, 4.0), but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly above extreme*/
    x[0] = 0.0;
    x[1] = -4.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 2.0 for dimension 1 at (0.0, -4.0), but returned %f\n", evalRet);
    x[0] = 1.0;
    x[1] = -6.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at (1.0, -6.0), but returned %f\n", evalRet);
    x[0] = 2.5;
    x[1] = -4.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at (2.5, -4.0), but returned %f\n", evalRet);
    x[0] = 0.5;
    x[1] = -3.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(2.0+1.0/6.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 2.16666 for dimension 1 at (0.5, -3.0), but returned %f\n", evalRet);
    x[0] = 3.0;
    x[1] = -2.5;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.5 for dimension 1 at (3.0, -2.5), but returned %f\n", evalRet);
    x[0] = 0.5;
    x[1] = -3.75;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.5 for dimension 1 at (0.5, -3.75), but returned %f\n", evalRet);
    x[0] = 0.5;
    x[1] = -3.5625;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(1.75 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.75 for dimension 1 at (0.5, -3.5625), but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly below extreme*/
    x[0] = 0.5;
    x[1] = -3.75;
    evalRet = ikSurf_eval(surf, 1, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at (0.5, -3.75), but returned %f\n", evalRet);
    x[0] = 0.5;
    x[1] = -3.8;
    evalRet = ikSurf_eval(surf, 1, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.0 for dimension 1 at (0.5, -3.8), but returned %f\n", evalRet);
    x[0] = 2.5;
    x[1] = -3.0;
    evalRet = ikSurf_eval(surf, 1, x, -1);
    if (fabs(0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 0.5 for dimension 1 at (2.5, -3.0), but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly above extreme*/
    x[0] = 0.5;
    x[1] = -3.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at (0.5, -3.5), but returned %f\n", evalRet);
    x[0] = 0.5;
    x[1] = -3.51;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(2.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 2.0 for dimension 0 at (0.5, -3.51), but returned %f\n", evalRet);
    x[0] = 2.0;
    x[1] = -3.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.5 for dimension 0 at (2.0, -3.5), but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly below extreme*/
    x[0] = 0.5;
    x[1] = -3.5;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 1.0 for dimension 0 at (0.5, -3.5), but returned %f\n", evalRet);
    x[0] = 0.5;
    x[1] = -2.625;
    evalRet = ikSurf_eval(surf, 0, x, -1);
    if (fabs(0.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=eval was expected to return 0.5 for dimension 0 at (0.5, -2.625), but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=3*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (3 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=getDimensions was expected to return 3, but returned %d\n", getDimensionsRet);

    /*getPointNumber=4 for dim 0 and getPointNumber=4 dim 1 and getPointNumber=1 for dim 2, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex (ikSurf_test) message=getPointNumber was expected to return 4 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (4 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=getPointNumber was expected to return 4 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 3);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 3, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=three D sixteen point convex (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test four-dimensional surface, i.e. euclidean space, with eight points.
 */
void testFourDimEightPoint() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test four D eight point\n");

    /*won't force an error*/
    printf("new\n");
    dims = 4;
    ndata = (int *) malloc(sizeof(int)*3);
    ndata[0] = 2;
    ndata[1] = 2;
    ndata[2] = 2;
    data = (double*) malloc(sizeof(double)*14);
    data[0] = 1.0;
    data[1] = 2.0;
    data[2] = 10.0;
    data[3] = 20.0;
    data[4] = 100.0;
    data[5] = 200.0;
    data[6] = 1.0;
    data[7] = 2.0;
    data[8] = 3.0;
    data[9] = 4.0;
    data[10] = 5.0;
    data[11] = 6.0;
    data[12] = 7.0;
    data[13] = 8.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=four D eight point (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 3-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*3);
    x[0] = 1.0;
    x[1] = 10.0;
    x[2] = 100.0;
    evalRet = ikSurf_eval(surf, 3, x, 0);
    if (fabs(1.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=eval was expected to return 1.0 for dimension 3 at (1.0, 10.0, 100.0), but returned %f\n", evalRet);
    x[0] = 1.5;
    x[1] = 15.0;
    x[2] = 150.0;
    evalRet = ikSurf_eval(surf, 3, x, 0);
    if (fabs(4.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=eval was expected to return 4.5 for dimension 3 at (1.5, 15.0, 150.0), but returned %f\n", evalRet);
    /*eval 2-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*3);
    x[0] = 1.0;
    x[1] = 10.0;
    x[2] = 0.5;
    evalRet = ikSurf_eval(surf, 2, x, 0);
    if (fabs(50.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=eval was expected to return 50.0 for dimension 2 at (1.0, 10.0, 0.5), but returned %f\n", evalRet);
    /*eval 1-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*3);
    x[0] = 1.5;
    x[1] = 150.0;
    x[2] = 4.5;
    evalRet = ikSurf_eval(surf, 1, x, 0);
    if (fabs(15.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=eval was expected to return 15.0 for dimension 1 at (1.5, 150.0, 4.5), but returned %f\n", evalRet);
    /*eval 0-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*3);
    x[0] = 15.0;
    x[1] = 150.0;
    x[2] = 4.5;
    evalRet = ikSurf_eval(surf, 0, x, 0);
    if (fabs(1.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=eval was expected to return 1.5 for dimension 0 at (15.0, 150.0, 4.5), but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=4*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (4 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=getDimensions was expected to return 4, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=2 dim 1 and getPointNumber=2 dim 2 and getPointNumber=1 for dim 3, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (2 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point (ikSurf_test) message=getPointNumber was expected to return 2 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (2 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=getPointNumber was expected to return 2 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (2 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=getPointNumber was expected to return 2 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 3);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point (ikSurf_test) message=getPointNumber was expected to return 1 for dimension 3, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 4);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 4, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

/**
 * Test instance construction from file data
 */
void testNewFromFile() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*declare file*/
    FILE *f = NULL;

    /*start test*/
    printf("ikSurf_test new from file\n");

    /*two-point line*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int)*1);
    ndata[0] = 2;
    data = (double*) malloc(sizeof(double)*4);
    data[0] = 12.0;
    data[1] = 24.0;
    data[2] = 6.0;
    data[3] = 7.0;
    f = fopen("ikSurf_test.1", "wb");
    if (NULL == f) {
      printf("%%TEST_FAILED%% time=0 testname=new from file (ikSurf_test) message=could not write file 1\n");
      free(ndata);
      free(data);
      return;
    }
    fwrite(&dims, sizeof(int), 1, f);
    fwrite(ndata, sizeof(int), 1, f);
    fwrite(data, sizeof(double), 4, f);
    fclose(f);
    f = NULL;
    err = ikSurf_newf(&surf, "ikSurf_test.1");
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new from file (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 1-th coordinate linearly*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double)*1);
    x[0] = 18.0;
    evalRet = ikSurf_eval(surf, 1, x, 0);
    if (fabs(6.5 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=new from file (ikSurf_test) message=eval was expected to return 6.5 for dimension 1 at 18.0, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=new from file(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=2 for dim 0 and getPointNumber=1 for dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (2 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point (ikSurf_test) message=getPointNumber was expected to return 2 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point(ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=four D eight point (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

void testNewErrors() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;

    /*start test*/
    printf("ikSurf_test new errors\n");

    /*get some good arguments*/
    dims = 4;
    ndata = (int *) malloc(sizeof(int)*3);
    ndata[0] = 2;
    ndata[1] = 2;
    ndata[2] = 2;
    data = (double*) malloc(sizeof(double)*14);
    data[0] = 1.0;
    data[1] = 2.0;
    data[2] = 10.0;
    data[3] = 20.0;
    data[4] = 100.0;
    data[5] = 200.0;
    data[6] = 1.0;
    data[7] = 2.0;
    data[8] = 3.0;
    data[9] = 4.0;
    data[10] = 5.0;
    data[11] = 6.0;
    data[12] = 7.0;
    data[13] = 8.0;

    /*force "bad dims"*/
    printf("bad dims\n");
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    dims = 0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad dims")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bad dims\", but returned \"%s\"\n", err);
    if (NULL != surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a NULL pointer, but didn't\n");
    dims = 4;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    dims = -1;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad dims")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bad dims\", but returned \"%s\"\n", err);
    if (NULL != surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a NULL pointer, but didn't\n");
    dims = 4;

    /*force "bad ndata"*/
    printf("bad ndata\n");
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    ndata[0] = 0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad ndata")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada ndata[0]\", but returned \"%s\"\n", err);
    if (NULL != surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a NULL pointer, but didn't\n");
    ndata[0] = 2;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    ndata[0] = -1;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad ndata")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada ndata[0]\", but returned \"%s\"\n", err);
    if (NULL != surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a NULL pointer, but didn't\n");
    ndata[0] = 2;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    ndata[1] = -1;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad ndata")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada ndata\", but returned \"%s\"\n", err);
    if (NULL != surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a NULL pointer, but didn't\n");
    ndata[1] = 2;
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;

    /*force "bad data"*/
    printf("bad data\n");
    dims = 3;
    ndata = (int *) malloc(sizeof(int)*2);
    ndata[0] = 4;
    ndata[1] = 4;
    data = (double*) malloc(sizeof(double)*24);
    data[0] = 1.0;
    data[1] = 2.0;
    data[2] = 3.0;
    data[3] = 4.0;
    data[4] = 1.0;
    data[5] = 2.0;
    data[6] = 3.0;
    data[7] = 4.0;
    data[8] = 1.0;
    data[9] = 2.0;
    data[10] = 3.0;
    data[11] = 2.0;
    data[12] = 2.0;
    data[13] = 3.0;
    data[14] = 4.0;
    data[15] = 3.0;
    data[16] = 3.0;
    data[17] = 3.5;
    data[18] = 5.0;
    data[19] = 3.5;
    data[20] = 2.0;
    data[21] = 3.0;
    data[22] = 4.0;
    data[23] = 3.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[0] = 3.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad data (not ascending)")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada data (not ascending)\", but returned \"%s\"\n", err);
    if (NULL != surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a NULL pointer, but didn't\n");
    data[0] = 1.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[6] = 1.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad data (not ascending)")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada data (not ascending)\", but returned \"%s\"\n", err);
    if (NULL != surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a NULL pointer, but didn't\n");
    data[6] = 3.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[8] = 3.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad data (neither concave nor convex)")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada data (neither concave nor convex)\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    data[8] = 1.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[13] = 1.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad data (neither concave nor convex)")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada data (neither concave nor convex)\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    data[13] = 3.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[22] = 2.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad data (neither concave nor convex)")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada data (neither concave nor convex)\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    data[22] = 4.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[18] = 3.75;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad data (neither concave nor convex)")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bada data (neither concave nor convex)\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    data[18] = 5.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[18] = 4.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    ikSurf_delete(surf);
    data[18] = 5.0;
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;

    /*see that "bad data (neither concave nor convex)" makes eval return 0.0 for all but the last dimension*/
    printf("bad data interp only\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int)*1);
    ndata[0] = 8;
    data = (double*) malloc(sizeof(double)*24);
    data[0] = 1.0;
    data[1] = 2.0;
    data[2] = 3.0;
    data[3] = 4.0;
    data[4] = 5.0;
    data[5] = 6.0;
    data[6] = 7.0;
    data[7] = 8.0;
    data[8] = 10.0;
    data[9] = 12.0;
    data[10] = 11.0;
    data[11] = 13.0;
    data[12] = 12.0;
    data[13] = 14.0;
    data[14] = 13.0;
    data[15] = 15.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    if (strcmp(err, "bad data (neither concave nor convex)")) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to return \"bad data (neither concave nor conves)\", but returned \"%s\"\n", err);
    if (NULL == surf) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=init was expected to pass a non-NULL pointer, but didn't\n");
    x = (double *) malloc(sizeof(double)*1);
    x[0] = 5.5;
    evalRet = ikSurf_eval(surf, 0, x, 1);
    if (fabs(0.0 - evalRet) > 1.0E-09) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=eval was expected to return 0.0 for dimension 0 at 5.5, but returned %f\n", evalRet);
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(13.0 - evalRet) > 1.0E-09) printf("%%TEST_FAILED%% time=0 testname=new errors (ikSurf_test) message=eval was expected to return 13.0 for dimension 0 at 5.5, but returned %f\n", evalRet);
    ikSurf_delete(surf);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    free(x);
    x = NULL;
}

/**
 * Test for bug that makes coordinate bisection fail when the coordinate value we seek coincides with the bisection midpoint
 */
void testBugCoordinateBisection() {
    /*declare instance reference*/
    ikSurf *surf = NULL;

    /*declare dims, ndata and data */
    int dims;
    int *ndata = NULL;
    double *data = NULL;

    /*declare error message, eval coordinates, other return values*/
    const char *err = NULL;
    double *x = NULL;
    double evalRet;
    int getDimensionsRet;
    int getPointNumberRet;

    /*start test*/
    printf("ikSurf_test bug 1\n");

    /*won't force an error*/
    printf("new\n");
    dims = 2;
    ndata = (int *) malloc(sizeof(int));
    ndata[0] = 5;
    data = (double*) malloc(sizeof(double)*10);
    data[0] = 7.0;
    data[1] = 8.0;
    data[2] = 9.0;
    data[3] = 10.0;
    data[4] = 11.0;
    data[5] = 9.0;
    data[6] = 19.0;
    data[7] = 4.0;
    data[8] = 3.0;
    data[9] = -9.0;
    err = ikSurf_new(&surf, dims, ndata, data, 1);
    free(ndata);
    ndata = NULL;
    free(data);
    data = NULL;
    if (strcmp(err, "")) printf("%%TEST_FAILED%% time=0 testname=bug 1 (ikSurf_test) message=init was expected to return \"\", but returned \"%s\"\n", err);

    /*eval 1-th coordinate linearly*/
    /*make it catch 10.0 just at the midle*/
    printf("eval\n");
    x = (double *) malloc(sizeof(double));
    x[0] = 7.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(9.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 9.0 for dimension 1 at 7.0, but returned %f\n", evalRet);
    x[0] = 10.0;
    evalRet = ikSurf_eval(surf, 1, x, 1);
    if (fabs(3.0 - evalRet) > 1.0E-9) printf("%%TEST_FAILED%% time=0 testname=two D three point(ikSurf_test) message=eval was expected to return 3.0 for dimension 1 at 10.0, but returned %f\n", evalRet);
    free(x);
    x = NULL;

    /*getDimensions=2*/
    printf("getDimensions\n");
    getDimensionsRet = ikSurf_getDimensions(surf);
    if (2 != getDimensionsRet) printf("%%TEST_FAILED%% time=0 testname=bug 1(ikSurf_test) message=getDimensions was expected to return 2, but returned %d\n", getDimensionsRet);

    /*getPointNumber=5 for dim 0 and getPointNumber=1 dim 1, 0 for all others*/
    printf("getPointNumber\n");
    getPointNumberRet = ikSurf_getPointNumber(surf, 0);
    if (5 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=bug 1 (ikSurf_test) message=getPointNumber was expected to return 5 for dimension 0, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 1);
    if (1 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=bug 1(ikSurf_test) message=getPointNumber was expected to return 1 for dimension 1, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, 2);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=bug 1 (ikSurf_test) message=getPointNumber was expected to return 0 for dimension 2, but returned %d\n", getPointNumberRet);
    getPointNumberRet = ikSurf_getPointNumber(surf, -1);
    if (0 != getPointNumberRet) printf("%%TEST_FAILED%% time=0 testname=bug 1 (ikSurf_test) message=getPointNumber was expected to return 0 for dimension -1, but returned %d\n", getPointNumberRet);

    /*release the memory*/
    printf("delete\n");
    ikSurf_delete(surf);
    surf = NULL;
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikSurf_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% one dim (ikSurf_test)\n");
    testOneDim();
    printf("%%TEST_FINISHED%% time=0 one dim (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim one point (ikSurf_test)\n");
    testTwoDimOnePoint();
    printf("%%TEST_FINISHED%% time=0 two dim one point (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim two point (ikSurf_test)\n");
    testTwoDimTwoPoint();
    printf("%%TEST_FINISHED%% time=0 two dim two point (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim three point (ikSurf_test)\n");
    testTwoDimThreePoint();
    printf("%%TEST_FINISHED%% time=0 two dim three point (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim three point convex (ikSurf_test)\n");
    testTwoDimThreePointConvex();
    printf("%%TEST_FINISHED%% time=0 two dim three point convex (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim three point concave (ikSurf_test)\n");
    testTwoDimThreePointConcave();
    printf("%%TEST_FINISHED%% time=0 two dim three point concave (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim four point (ikSurf_test)\n");
    testTwoDimFourPoint();
    printf("%%TEST_FINISHED%% time=0 two dim four point (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim four point convex (ikSurf_test)\n");
    testTwoDimFourPointConvex();
    printf("%%TEST_FINISHED%% time=0 two dim four point convex (ikSurf_test) \n");

    printf("%%TEST_STARTED%% two dim four point concave (ikSurf_test)\n");
    testTwoDimFourPointConcave();
    printf("%%TEST_FINISHED%% time=0 two dim four point concave (ikSurf_test) \n");

    printf("%%TEST_STARTED%% three dim four point (ikSurf_test)\n");
    testThreeDimFourPoint();
    printf("%%TEST_FINISHED%% time=0 three dim four point (ikSurf_test) \n");

    printf("%%TEST_STARTED%% three dim sixteen point (ikSurf_test)\n");
    testThreeDimSixteenPoint();
    printf("%%TEST_FINISHED%% time=0 three dim sixteen point (ikSurf_test) \n");

    printf("%%TEST_STARTED%% three dim sixteen point convex (ikSurf_test)\n");
    testThreeDimSixteenPointConvex();
    printf("%%TEST_FINISHED%% time=0 three dim sixteen point convex (ikSurf_test) \n");

    printf("%%TEST_STARTED%% three dim sixteen point concave (ikSurf_test)\n");
    testThreeDimSixteenPointConcave();
    printf("%%TEST_FINISHED%% time=0 three dim sixteen point concave (ikSurf_test) \n");

    printf("%%TEST_STARTED%% four dim eight point (ikSurf_test)\n");
    testFourDimEightPoint();
    printf("%%TEST_FINISHED%% time=0 four dim eight point (ikSurf_test) \n");

    printf("%%TEST_STARTED%% new from file (ikSurf_test)\n");
    testNewFromFile();
    printf("%%TEST_FINISHED%% time=0 new from file (ikSurf_test) \n");

    printf("%%TEST_STARTED%% new errors (ikSurf_test)\n");
    testNewErrors();
    printf("%%TEST_FINISHED%% time=0 new errors (ikSurf_test) \n");

    printf("%%TEST_STARTED%% bug in coordinate bisection (ikSurf_test)\n");
    testBugCoordinateBisection();
    printf("%%TEST_FINISHED%% time=0 bug in coordinate bisection (ikSurf_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
