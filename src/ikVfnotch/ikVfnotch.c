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

#include "ikVfnotch.h"

/**
 * @file ikVfnotch.c
 * 
 * @brief Class ikVfnotch implementation 
 */

/* @cond */

/**
 * (Private static) get the discrete-time implementation parameters
 * @param dT sampling time
 * @param freq frequency
 * @param dampDen denominator damping coefficient
 * @param dampNum numerator damping coefficient
 * @param a discrete-time implementation denominator parameters, size 3
 * @param b discrete-time implementation numerator parameters, size 3
 * @return error code:
 *  0: no error
 * -1: invalid sampling time, must be positive
 * -2: invalid frequency, must be positive
 */
int ikVfnotch_c2d(double dT, double freq, double dampDen, double dampNum, double a[], double b[]) {
    int i;
    
    /*declare parameter array p == { a, b } */
    double p[6];
    /*declare damping coefficient array */
    double d[2];
    
    /*check sampling time */
    if (0 >= dT) return -1;
    
    /*check frequency */
    if (0 >= freq) return -2;

    d[0] = dampDen;
    d[1] = dampNum;

    /*calculate parameters */
    for (i = 0; i < 2; i++) {
        p[i * 3] = 4 + 4 * d[i] * dT * freq + dT * dT * freq*freq;       /*z^0 */
        p[i * 3 + 1] = -8 + 2 * dT * dT * freq*freq;                     /*z^-1 */
        p[i * 3 + 2] = 4 - 4 * d[i] * dT * freq + dT * dT * freq*freq;   /*z^-2 */
    }

    /*copy parameters to output arrays */
    for (i = 0; i < 3; i++) {
        a[i] = p[i];
        b[i] = p[i + 3];
    }
    
    /*return error code */
    return 0;
}

int ikVfnotch_init(ikVfnotch *self, double dT, double freq, double dampDen, double dampNum) {
    /*discretise */
    double a[3];
    double b[3];
    int err = ikVfnotch_c2d(dT, freq, dampDen, dampNum, a, b);
    
    /*return error code */
    if (0 != err) return err;

    /*initialise filter */
    ikSlti_init(&(self->filter));
    ikSlti_setParam(&(self->filter), a, b);
    
    /*register parameters */
    self->dT = dT;
    self->freq = freq;
    self->dampDen = dampDen;
    self->dampNum = dampNum;

    /*return error code */
    return 0;
}

int ikVfnotch_setSamplingTime(ikVfnotch *self, double dT) {
    /*re-discretise */
    double a[3];
    double b[3];
    int err = ikVfnotch_c2d(dT, self->freq, self->dampDen, self->dampNum, a, b);
    
    /*return error code */
    if (0 != err) return err;

    /*change filter parameters */
    ikSlti_setParam(&(self->filter), a, b);
    
    /*register new sampling time */
    self->dT = dT;

    /*return error code */
    return 0;
}

double ikVfnotch_getSamplingTime(const ikVfnotch *self) {
    /*return sampling time */
    return self->dT;
}

int ikVfnotch_setFreq(ikVfnotch *self, double freq) {
    /*re-discretise */
    double a[3];
    double b[3];
    int err = ikVfnotch_c2d(self->dT, freq, self->dampDen, self->dampNum, a, b);
    
    /*return error code */
    if (0 != err) return err;

    /*change filter parameters */
    ikSlti_setParam(&(self->filter), a, b);
    
    /*register new frequency */
    self->freq = freq;

    /*return error code */
    return 0;
}

double ikVfnotch_getFreq(const ikVfnotch *self) {
    /*return frequency */
    return self->freq;
}

void ikVfnotch_setDamp(ikVfnotch *self, double dampDen, double dampNum) {
    /*re-discretise */
    double a[3];
    double b[3];
    ikVfnotch_c2d(self->dT, self->freq, dampDen, dampNum, a, b);

    /*change filter parameters */
    ikSlti_setParam(&(self->filter), a, b);
    
    /*register new damping coefficients */
    self->dampDen = dampDen;
    self->dampNum = dampNum;

}

void ikVfnotch_getDamp(const ikVfnotch *self, double *dampDen, double *dampNum) {
    /*output damping coefficients */
    *dampDen = self->dampDen;
    *dampNum = self->dampNum;
}

double ikVfnotch_step(ikVfnotch *self, double input) {
    /*invoke filter's step method */
    return ikSlti_step(&(self->filter), input);
}

double ikVfnotch_getOutput(const ikVfnotch *self) {
    /*invoke filter's getOutput method */
    return ikSlti_getOutput(&(self->filter));
}

/* @endcond */
