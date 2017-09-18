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
 * @file ikRegionSelector_test.c
 * 
 * @brief Class ikRegionSelector unit tests
 */

#include <stdio.h>
#include <stdlib.h>
#include "../ikRegionSelector/ikRegionSelector.h"

/*
 * Simple C Test Suite
 */

/**
 * A rectangular region is correctly implemented
 */
void testRectangularRegion() {
    printf("ikRegionSelector_test testRectangularRegion\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* initialise instance */
    ikRegionSelector_initParams(&params);
    params.nRegions = 1;
    params.regions[0].nPoints = 4;
    params.regions[0].points[0].x = 1.0;
    params.regions[0].points[0].y = 1.0;
    params.regions[0].points[1].x = 1.0;
    params.regions[0].points[1].y = 2.0;
    params.regions[0].points[2].x = 2.0;
    params.regions[0].points[2].y = 2.0;
    params.regions[0].points[3].x = 2.0;
    params.regions[0].points[3].y = 1.0;
    err = ikRegionSelector_init(&rs, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testRectangularRegion (ikRegionSelector_test) message=expected init to return 0, but it returned %d\n", err);
    
    /* see that point {1.1, 1.1} is in */
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.1);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testRectangularRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.1,1.1}, but it returned %d\n", output);
    
    /* see that point {1.1, 0.9} is out */
    output = ikRegionSelector_getRegion(&rs, 1.1, 0.9);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testRectangularRegion (ikRegionSelector_test) message=expected getRegion to return 0 for point {1.1,0.9}, but it returned %d\n", output);
    
}

/**
 * A triangular region is correctly implemented
 */
void testTriangularRegion() {
    printf("ikRegionSelector_test testTriangularRegion\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* initialise instance */
    ikRegionSelector_initParams(&params);
    params.nRegions = 1;
    params.regions[0].nPoints = 3;
    params.regions[0].points[0].x = 1.0;
    params.regions[0].points[0].y = 1.0;
    params.regions[0].points[1].x = 2.0;
    params.regions[0].points[1].y = 1.0;
    params.regions[0].points[2].x = 2.0;
    params.regions[0].points[2].y = 2.0;
    err = ikRegionSelector_init(&rs, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testTriangularRegion (ikRegionSelector_test) message=expected init to return 0, but it returned %d\n", err);
    
    /* see that point {1.2, 1.1} is in */
    output = ikRegionSelector_getRegion(&rs, 1.2, 1.1);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testTriangularRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.2,1.1}, but it returned %d\n", output);
    
    /* see that point {1.1, 1.2} is out */
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.2);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testTriangularRegion (ikRegionSelector_test) message=expected getRegion to return 0 for point {1.1,1.2}, but it returned %d\n", output);
    
}

/**
 * An X-shaped region is correctly implemented
 */
void testXRegion() {
    printf("ikRegionSelector_test testXRegion\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* initialise instance */
    ikRegionSelector_initParams(&params);
    params.nRegions = 1;
    params.regions[0].nPoints = 4;
    params.regions[0].points[0].x = 1.0;
    params.regions[0].points[0].y = 1.0;
    params.regions[0].points[1].x = 1.0;
    params.regions[0].points[1].y = 2.0;
    params.regions[0].points[2].x = 2.0;
    params.regions[0].points[2].y = 1.0;
    params.regions[0].points[3].x = 2.0;
    params.regions[0].points[3].y = 2.0;
    err = ikRegionSelector_init(&rs, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testXRegion (ikRegionSelector_test) message=expected init to return 0, but it returned %d\n", err);
    
    /* see that point {1.1, 1.2} is in */
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.2);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testXRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.1,1.2}, but it returned %d\n", output);
    
    /* see that point {1.9, 1.2} is in */
    output = ikRegionSelector_getRegion(&rs, 1.9, 1.2);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testXRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.9,1.2}, but it returned %d\n", output);
    
    /* see that point {1.5, 1.2} is out */
    output = ikRegionSelector_getRegion(&rs, 1.5, 1.2);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testXRegion (ikRegionSelector_test) message=expected getRegion to return 0 for point {1.5, 1.2}, but it returned %d\n", output);
    
}

/**
 * A C-shaped region is correctly implemented
 */
void testCRegion() {
    printf("ikRegionSelector_test testCRegion\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* initialise instance */
    ikRegionSelector_initParams(&params);
    params.nRegions = 1;
    params.regions[0].nPoints = 8;
    params.regions[0].points[0].x = 1.0;
    params.regions[0].points[0].y = 1.0;
    params.regions[0].points[1].x = 1.0;
    params.regions[0].points[1].y = 4.0;
    params.regions[0].points[2].x = 3.0;
    params.regions[0].points[2].y = 4.0;
    params.regions[0].points[3].x = 3.0;
    params.regions[0].points[3].y = 3.0;
    params.regions[0].points[4].x = 2.0;
    params.regions[0].points[4].y = 3.0;
    params.regions[0].points[5].x = 2.0;
    params.regions[0].points[5].y = 2.0;
    params.regions[0].points[6].x = 3.0;
    params.regions[0].points[6].y = 2.0;
    params.regions[0].points[7].x = 3.0;
    params.regions[0].points[7].y = 1.0;
    err = ikRegionSelector_init(&rs, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testCRegion (ikRegionSelector_test) message=expected init to return 0, but it returned %d\n", err);
    
    /* see that point {1.1, 1.2} is in */
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.2);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testCRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.1,1.2}, but it returned %d\n", output);
    
    /* see that point {2.5, 3.5} is in */
    output = ikRegionSelector_getRegion(&rs, 2.5, 3.5);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testCRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {2.5, 3.5}, but it returned %d\n", output);
    
    /* see that point {2.5, 2.5} is out */
    output = ikRegionSelector_getRegion(&rs, 2.5, 2.5);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testCRegion (ikRegionSelector_test) message=expected getRegion to return 0 for point {2.5, 2.5}, but it returned %d\n", output);
    
}

/**
 * A self-overlapping region is correctly implemented
 */
void testSelfOverlappingRegion() {
    printf("ikRegionSelector_test testSelfOverlappingRegion\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* initialise instance */
    ikRegionSelector_initParams(&params);
    params.nRegions = 1;
    params.regions[0].nPoints = 10;
    params.regions[0].points[0].x = 1.0;
    params.regions[0].points[0].y = 1.0;
    params.regions[0].points[1].x = 1.0;
    params.regions[0].points[1].y = 4.0;
    params.regions[0].points[2].x = 4.0;
    params.regions[0].points[2].y = 4.0;
    params.regions[0].points[3].x = 4.0;
    params.regions[0].points[3].y = 3.0;
    params.regions[0].points[4].x = 2.0;
    params.regions[0].points[4].y = 3.0;
    params.regions[0].points[5].x = 2.0;
    params.regions[0].points[5].y = 2.0;
    params.regions[0].points[6].x = 3.0;
    params.regions[0].points[6].y = 2.0;
    params.regions[0].points[7].x = 3.0;
    params.regions[0].points[7].y = 4.0;
    params.regions[0].points[8].x = 4.0;
    params.regions[0].points[8].y = 4.0;
    params.regions[0].points[9].x = 4.0;
    params.regions[0].points[9].y = 1.0;
    err = ikRegionSelector_init(&rs, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testSelfOverlappingRegion (ikRegionSelector_test) message=expected init to return 0, but it returned %d\n", err);
    
    /* see that point {1.1, 1.2} is in */
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.2);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testSelfOverlappingRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.1,1.2}, but it returned %d\n", output);
    
    /* see that point {2.5, 3.5} is in */
    output = ikRegionSelector_getRegion(&rs, 2.5, 3.5);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testSelfOverlappingRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {2.5, 3.5}, but it returned %d\n", output);
    
    /* see that point {3.5, 3.5} is in */
    output = ikRegionSelector_getRegion(&rs, 3.5, 3.5);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testSelfOverlappingRegion (ikRegionSelector_test) message=expected getRegion to return 1 for point {3.5, 3.5}, but it returned %d\n", output);
    
    /* see that point {2.5, 2.5} is out */
    output = ikRegionSelector_getRegion(&rs, 2.5, 2.5);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testSelfOverlappingRegion (ikRegionSelector_test) message=expected getRegion to return 0 for point {2.5, 2.5}, but it returned %d\n", output);
    
}

/**
 * Two overlapping regions are correctly implemented
 */
void testOverlappingRegions() {
    printf("ikRegionSelector_test testOverlappingRegions\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* initialise instance */
    ikRegionSelector_initParams(&params);
    params.nRegions = 2;
    params.regions[0].nPoints = 4;
    params.regions[0].points[0].x = 1.0;
    params.regions[0].points[0].y = 1.0;
    params.regions[0].points[1].x = 1.0;
    params.regions[0].points[1].y = 2.0;
    params.regions[0].points[2].x = 2.0;
    params.regions[0].points[2].y = 2.0;
    params.regions[0].points[3].x = 2.0;
    params.regions[0].points[3].y = 1.0;
    params.regions[1].nPoints = 4;
    params.regions[1].points[0].x = 1.5;
    params.regions[1].points[0].y = 1.5;
    params.regions[1].points[1].x = 1.5;
    params.regions[1].points[1].y = 2.5;
    params.regions[1].points[2].x = 2.5;
    params.regions[1].points[2].y = 2.5;
    params.regions[1].points[3].x = 2.5;
    params.regions[1].points[3].y = 1.5;
    err = ikRegionSelector_init(&rs, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testOverlappingRegions (ikRegionSelector_test) message=expected init to return 0, but it returned %d\n", err);
    
    /* see that point {1.1, 1.1} is in region 1 */
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.1);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testOverlappingRegions (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.1,1.1}, but it returned %d\n", output);
    
    /* see that point {2.1, 2.1} is in region 2 */
    output = ikRegionSelector_getRegion(&rs, 2.1, 2.1);
    if (2 != output) printf("%%TEST_FAILED%% time=0 testname=testOverlappingRegions (ikRegionSelector_test) message=expected getRegion to return 2 for point {2.1, 2.1}, but it returned %d\n", output);
    
    /* see that point {1.9, 1.9} is in region 1 */
    output = ikRegionSelector_getRegion(&rs, 1.9, 1.9);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testOverlappingRegions (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.9, 1.9}, but it returned %d\n", output);
        
    /* see that point {1.1, 2.1} is out */
    output = ikRegionSelector_getRegion(&rs, 1.1, 2.1);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testOverlappingRegions (ikRegionSelector_test) message=expected getRegion to return 0 for point {1.1, 2.1}, but it returned %d\n", output);
    
}

/**
 * Bad arguments passed at initialisation result in the right error codes
 */
void testInitErrors() {
    printf("ikRegionSelector_test testInitErrors\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* -1 for too few regions, default behaviour */
    ikRegionSelector_initParams(&params);
    params.nRegions = -1;
    err = ikRegionSelector_init(&rs, &params);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected init to return -1, but it returned %d\n", err);
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.1);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected getRegion to return 0 for point {1.1, 1.1}, but it returned %d\n", output);
    
    /* -1 for too many regions, default behaviour */
    ikRegionSelector_initParams(&params);
    params.nRegions = IKREGIONSELECTOR_MAXREG + 1;
    err = ikRegionSelector_init(&rs, &params);
    if (-1 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected init to return -1, but it returned %d\n", err);
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.1);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected getRegion to return 0 for point {1.1, 1.1}, but it returned %d\n", output);
    
    /* 2 for too few or too many points in second region, normal behaviour for other regions */
    ikRegionSelector_initParams(&params);
    params.nRegions = 3;
    params.regions[0].nPoints = 4;
    params.regions[0].points[0].x = 1.0;
    params.regions[0].points[0].y = 1.0;
    params.regions[0].points[1].x = 1.0;
    params.regions[0].points[1].y = 2.0;
    params.regions[0].points[2].x = 2.0;
    params.regions[0].points[2].y = 2.0;
    params.regions[0].points[3].x = 2.0;
    params.regions[0].points[3].y = 1.0;
    params.regions[1].nPoints = -1;
    params.regions[2].nPoints = 4;
    params.regions[2].points[0].x = 1.5;
    params.regions[2].points[0].y = 1.5;
    params.regions[2].points[1].x = 1.5;
    params.regions[2].points[1].y = 2.5;
    params.regions[2].points[2].x = 2.5;
    params.regions[2].points[2].y = 2.5;
    params.regions[2].points[3].x = 2.5;
    params.regions[2].points[3].y = 1.5;
    err = ikRegionSelector_init(&rs, &params);
    if (2 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected init to return 2, but it returned %d\n", err);
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.1);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.1,1.1}, but it returned %d\n", output);
    output = ikRegionSelector_getRegion(&rs, 2.1, 2.1);
    if (3 != output) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected getRegion to return 3 for point {2.1, 2.1}, but it returned %d\n", output);
    params.regions[1].nPoints = IKREGIONSELECTOR_MAXPOINTS + 1;
    err = ikRegionSelector_init(&rs, &params);
    if (2 != err) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected init to return 2, but it returned %d\n", err);
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.1);
    if (1 != output) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected getRegion to return 1 for point {1.1,1.1}, but it returned %d\n", output);
    output = ikRegionSelector_getRegion(&rs, 2.1, 2.1);
    if (3 != output) printf("%%TEST_FAILED%% time=0 testname=testInitErrors (ikRegionSelector_test) message=expected getRegion to return 3 for point {2.1, 2.1}, but it returned %d\n", output);
    
}

/**
 * Default behaviour is as expected
 */
void testDefault() {
    printf("ikRegionSelector_test testDefault\n");
    /* declare error code */
    int err;
    /* declare output value */
    int output;
    /* declare instance */
    ikRegionSelector rs;
    /* declare initialisation parameters */
    ikRegionSelectorParams params;
    
    /* initialise instance */
    ikRegionSelector_initParams(&params);
    err = ikRegionSelector_init(&rs, &params);
    if (err) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikRegionSelector_test) message=expected init to return 0, but it returned %d\n", err);
    
    /* see that point {1.1, 1.1} is in region 0 (all points are) */
    output = ikRegionSelector_getRegion(&rs, 1.1, 1.1);
    if (0 != output) printf("%%TEST_FAILED%% time=0 testname=testDefault (ikRegionSelector_test) message=expected getRegion to return 0 for point {1.1,1.1}, but it returned %d\n", output);
    
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% ikRegionSelector_test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% testRectangularRegion (ikRegionSelector_test)\n");
    testRectangularRegion();
    printf("%%TEST_FINISHED%% time=0 testRectangularRegion (ikRegionSelector_test) \n");

    printf("%%TEST_STARTED%% testTriangularRegion (ikRegionSelector_test)\n");
    testTriangularRegion();
    printf("%%TEST_FINISHED%% time=0 testTriangularRegion (ikRegionSelector_test) \n");

    printf("%%TEST_STARTED%% testXRegion (ikRegionSelector_test)\n");
    testXRegion();
    printf("%%TEST_FINISHED%% time=0 testXRegion (ikRegionSelector_test) \n");

    printf("%%TEST_STARTED%% testCRegion (ikRegionSelector_test)\n");
    testCRegion();
    printf("%%TEST_FINISHED%% time=0 testCRegion (ikRegionSelector_test) \n");

    printf("%%TEST_STARTED%% testSelfOverlappingRegion (ikRegionSelector_test)\n");
    testSelfOverlappingRegion();
    printf("%%TEST_FINISHED%% time=0 testSelfOverlappingRegion (ikRegionSelector_test) \n");

    printf("%%TEST_STARTED%% testOverlappingRegions (ikRegionSelector_test)\n");
    testOverlappingRegions();
    printf("%%TEST_FINISHED%% time=0 testOverlappingRegions (ikRegionSelector_test) \n");

    printf("%%TEST_STARTED%% testInitErrors (ikRegionSelector_test)\n");
    testInitErrors();
    printf("%%TEST_FINISHED%% time=0 testInitErrors (ikRegionSelector_test) \n");

    printf("%%TEST_STARTED%% testDefault (ikRegionSelector_test)\n");
    testDefault();
    printf("%%TEST_FINISHED%% time=0 testDefault (ikRegionSelector_test) \n");
    
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
