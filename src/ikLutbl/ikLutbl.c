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
 * @file ikLutbl.c
 * 
 * @brief Class ikLutbl implementation
 */

/* @cond */

#include <math.h>
#include "ikLutbl.h"

void ikLutbl_init(ikLutbl *self) {
    /*initialise members */
    self->m = 1;
    self->x[0] = 0.0;
    self->y[0] = 1.0;
}

int ikLutbl_getPointNumber(ikLutbl *self) {
    return self->m;
}

int ikLutbl_setPoints(ikLutbl *self, int m, const double x[], const double y[]) {
    int i;
    
    /*check m */
    if (m > IKLUTBL_MAXPOINTS) return -1;
    if (m < 1) return -1;

    /*set m */
    self->m = m;

    /*check sortedness */
    for (i = 1; i < m; i++) {
        if (x[i] <= x[i - 1]) return -2;
    }

    /*set m points */
    for (i = 0; i < m; i++) {
        self->x[i] = x[i];
        self->y[i] = y[i];
    }

    /*return error code */
    return 0;
}

int ikLutbl_getPoints(const ikLutbl *self, int m, double x[], double y[]) {
    int i;
    
    /*check m */
    if (m > self->m) return -1;

    /*output m values */
    for (i = 0; i < m; i++) {
        x[i] = self->x[i];
        y[i] = self->y[i];
    }

    /*return error code */
    return 0;
}

double ikLutbl_eval(const ikLutbl *self, double x) {
    int i;
    int j;
    
    /*declare the middle point */
    int k;
    
    /*if there is only one point, return its y value */
    if (1 == self->m) return self->y[0];

    /*find the index of the table point which is closest to x from above */
    /*put upper limit at m-1 */
    i = self->m - 1;
    /*put lower limit at 1 */
    j = 1;
    /*check for extrapolation cases */
    if (x <= self->x[1]) i = 1;
    if (x >= self->x[self->m - 1]) j = self->m - 1;
    while (1) {
        /*see if we already have a solution */
        if (i <= j+1) break;
        /*put middle point in the middle, or 0.5 lower if the middle is not int */
        k = (int) (floor((i + j) / 2 + 0.1));
        /*choose sides */
        if (x <= self->x[k]) i = k;
        if (x >= self->x[k]) j = k;
    }

    /*interpolate/extrapolate using upper limit i */
    return self->y[i - 1] + (x - self->x[i - 1]) * (self->y[i] - self->y[i - 1]) / (self->x[i] - self->x[i - 1]);
}

/* @endcond */
