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
 * @file ikVector_test.c
 * 
 * @brief Class ikVector unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ikVector.h"

/*
 * Simple C Test Suite
 */

void testAdd() {
    printf("ikVector_test testAdd\n");
    
    ikVector u, v, r;
    
    u.c[0] = 14.0;
    u.c[1] = 24.0;
    u.c[2] = 37.0;
    v.c[0] = 3.0;
    v.c[1] = 5.0;
    v.c[2] = -2.0;
    
    r = ikVector_add(u, v);
    
    if (fabs(r.c[0] - 17.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testAdd (ikVector_test) message=coordinate 0 expected to be 17.0, but is %f\n", r.c[0]);
    if (fabs(r.c[1] - 29.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testAdd (ikVector_test) message=coordinate 1 expected to be 29.0, but is %f\n", r.c[1]);
    if (fabs(r.c[2] - 35.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testAdd (ikVector_test) message=coordinate 2 expected to be 35.0, but is %f\n", r.c[2]);
}

void testMult() {
    printf("ikVector_test testMult\n");
    
    ikVector u, r;
    
    u.c[0] = 14.0;
    u.c[1] = 24.0;
    u.c[2] = 37.0;
    
    r = ikVector_mult(u, -2.0);
    
    if (fabs(r.c[0] + 28.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testMult (ikVector_test) message=coordinate 0 expected to be -28.0, but is %f\n", r.c[0]);
    if (fabs(r.c[1] + 48.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testMult (ikVector_test) message=coordinate 1 expected to be -48.0, but is %f\n", r.c[1]);
    if (fabs(r.c[2] + 74.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testMult (ikVector_test) message=coordinate 2 expected to be -74.0, but is %f\n", r.c[2]);
}

void testDot() {
    printf("ikVector_test testDot\n");
    
    ikVector u, v;
    
    double r;
    
    u.c[0] = 2.0;
    u.c[1] = 4.0;
    u.c[2] = 10.0;
    v.c[0] = 0.5;
    v.c[1] = 0.25;
    v.c[2] = 0.1;
    
    r = ikVector_dot(u, v);
    
    if (fabs(r -3.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testDot (ikVector_test) message=dot product expected to be 3.0, but is %f\n", r);
}

void testCross() {
    printf("ikVector_test testCross\n");
    
    ikVector u, v, r;
    
    u.c[0] = 1.0;
    u.c[1] = 1.0;
    u.c[2] = 1.0;
    v.c[0] = -1.0;
    v.c[1] = 1.0;
    v.c[2] = 1.0;
    
    r = ikVector_cross(u, v);
    
    if (fabs(r.c[0] - 0.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testCross (ikVector_test) message=coordinate 0 expected to be 0.0, but is %f\n", r.c[0]);
    if (fabs(r.c[1] + 2.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testCross (ikVector_test) message=coordinate 1 expected to be -2.0, but is %f\n", r.c[1]);
    if (fabs(r.c[2] - 2.0) > 1e-9) printf("%%TEST_FAILED%% time=0 testname=testCross (ikVector_test) message=coordinate 2 expected to be 2.0, but is %f\n", r.c[2]);
}

void testRotate() {
    printf("ikVector_test testRotate\n");
    
    ikVector u, v, r;
    
    u.c[0] = 1.0;
    u.c[1] = 0.0;
    u.c[2] = 0.0;
    v.c[0] = -1.0;
    v.c[1] = 1.0;
    v.c[2] = 1.0;
    
    v = ikVector_mult(v, 3.14159265358979 * 2.0/3.0/sqrt(3.0)); /* This is a third of a turn */
    
    r = ikVector_rotate(u, v);
    
    if (fabs(r.c[0] - 0.0) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=testRotate (ikVector_test) message=coordinate 0 expected to be 0.0, but is %f\n", r.c[0]);
    if (fabs(r.c[1] - 0.0) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=testRotate (ikVector_test) message=coordinate 1 expected to be 0.0, but is %f\n", r.c[1]);
    if (fabs(r.c[2] + 1.0) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=testRotate (ikVector_test) message=coordinate 2 expected to be -1.0, but is %f\n", r.c[2]);

    v.c[0] = 0.0;
    v.c[1] = 0.0;
    v.c[2] = 0.0;

    r = ikVector_rotate(u, v);

    if (fabs(r.c[0] - 1.0) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=testRotate (ikVector_test) message=coordinate 0 expected to be 1.0, but is %f\n", r.c[0]);
    if (fabs(r.c[1] - 0.0) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=testRotate (ikVector_test) message=coordinate 1 expected to be 0.0, but is %f\n", r.c[1]);
    if (fabs(r.c[2] - 0.0) > 1e-6) printf("%%TEST_FAILED%% time=0 testname=testRotate (ikVector_test) message=coordinate 2 expected to be 0.0, but is %f\n", r.c[2]);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikVector_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testAdd (ikVector_test)\n");
    testAdd();
    printf("%%TEST_FINISHED%% time=0 testAdd (ikVector_test) \n");

    printf("%%TEST_STARTED%% testMult (ikVector_test)\n");
    testMult();
    printf("%%TEST_FINISHED%% time=0 testMult (ikVector_test) \n");

    printf("%%TEST_STARTED%% testDot (ikVector_test)\n");
    testDot();
    printf("%%TEST_FINISHED%% time=0 testDot (ikVector_test) \n");

    printf("%%TEST_STARTED%% testCross (ikVector_test)\n");
    testCross();
    printf("%%TEST_FINISHED%% time=0 testCross (ikVector_test) \n");

    printf("%%TEST_STARTED%% testRotate (ikVector_test)\n");
    testRotate();
    printf("%%TEST_FINISHED%% time=0 testRotate (ikVector_test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
