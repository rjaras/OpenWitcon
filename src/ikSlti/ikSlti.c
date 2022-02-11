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
 * @file ikSlti.c
 * 
 * @brief class ikSlti implementation
 */

/* @cond */

#include <math.h>
#include "ikSlti.h"

void ikSlti_init(ikSlti *self) {    
    /*set member values */
    int i;
    for (i = 0; i < 3; i++) {
        self->a[i] = 0.0;
        self->b[i] = 0.0;
        self->inBuff[i] = 0.0;
        self->outBuff[i] = 0.0;
    }
    self->a[0] = 1.0;
    self->b[0] = 1.0;
    self->suma = 1.0;
    self->sumb = 1.0;
    self->inSat = 0;
    self->outSat = 0;
    self->inMin = 0.0;
    self->inMax = 0.0;
    self->outMin = 0.0;
    self->outMax = 0.0;
}

int ikSlti_setParam(ikSlti *self, const double a[], const double b[]) {  
    int i;
    
    /*check a[0] is non-zero] */
    if (0.0 == a[0]) return -1;
    
    /*copy a and b values and compute sums*/
    self->suma = 0.0;
    self->sumb = 0.0;
    for (i = 0; i < 3; i++) {
        self->a[i] = a[i];
        self->b[i] = b[i];
        self->suma += a[i];
        self->sumb += b[i];
    }
    return 0;
}

void ikSlti_setBuff(ikSlti *self, const double inBuff[], const double outBuff[]) {
    /*copy inBuff and outBuff values */
    int i;
    for (i = 0; i < 3; i++) {
        self->inBuff[i] = inBuff[i];
        self->outBuff[i] = outBuff[i];
    }
}

void ikSlti_getParam(const ikSlti *self, double a[], double b[]) {
    /*copy self->a and self->b values */
    int i;
    for (i = 0; i < 3; i++) {
        a[i] = self->a[i];
        b[i] = self->b[i];
    }
}

void ikSlti_getBuff(const ikSlti *self, double inBuff[], double outBuff[]) {
    /*copy self->inBuff and self-outBuff values */
    int i;
    for (i = 0; i < 3; i++) {
        inBuff[i] = self->inBuff[i];
        outBuff[i] = self->outBuff[i];
    }
}

int ikSlti_setInSat(ikSlti *self, int enable, double min, double max) {
    /*check that enable is valid */
    if ((-1 > enable) || (2 < enable)) return -1;
    
    /*check that min and max make sense */
    if ((2 == enable) && (min > max)) return -2;
    
    /*register input values */
    self->inSat = enable;
    self->inMin = min;
    self->inMax = max;
    
    /*return error code */
    return 0;
}

int ikSlti_getInSat(const ikSlti *self, double *min, double *max) {
    /*output values */
    *min = self->inMin;
    *max = self->inMax;
    
    /*return value */
    return self->inSat;
}

int ikSlti_setOutSat(ikSlti *self, int enable, double min, double max) {
    /*check that enable is valid */
    if ((-1 > enable) || (2 < enable)) return -1;
    
    /*check that min and max make sense */
    if ((2 == enable) && (min > max)) return -2;
    
    /*register input values */
    self->outSat = enable;
    self->outMin = min;
    self->outMax = max;
    
    /*return error code */
    return 0;
}

int ikSlti_getOutSat(const ikSlti *self, double *min, double *max) {
    /*output values */
    *min = self->outMin;
    *max = self->outMax;
    
    /*return value */
    return self->outSat;
}

double ikSlti_step(ikSlti *self, double input) {
    int i;
    
    /*move old values down the buffers */
    for (i = 2; i > 0; i--) {
        self->inBuff[i] = self->inBuff[i-1];
        self->outBuff[i] =self->outBuff[i-1];
    }
    
    /*register new input */
    self->inBuff[0] = input;
    
    /*apply input saturation */
    if ((-1 == self->inSat) || (2 == self->inSat)) 
        if (self->inMin > self->inBuff[0]) self->inBuff[0] = self->inMin;
    if ((1 == self->inSat) || (2 == self->inSat))
        if (self->inMax < self->inBuff[0]) self->inBuff[0] = self->inMax;
    
    /*compute new output value */
    self->outBuff[0] = self->b[0] * self->inBuff[0];
    for (i = 1; i < 3; i++) {
        self->outBuff[0] -= self->a[i] * self->outBuff[i];
        self->outBuff[0] += self->b[i] * self->inBuff[i];
    }
    self->outBuff[0] = self->outBuff[0] / self->a[0];
    
    /*apply output saturation */
    if ((-1 == self->outSat) || (2 == self->outSat)) {
        if (self->outMin > self->outBuff[0]) {
            for (i = 0; i < 3; i++) {
                self->outBuff[i] = self->outMin;
                if (0.0 != self->sumb) self->inBuff[i] = self->outMin/self->sumb*self->suma;
            }
        }
    }
    if ((1 == self->outSat) || (2 == self->outSat)) {
        if (self->outMax < self->outBuff[0]) {
            for (i = 0; i < 3; i++) {
                self->outBuff[i] = self->outMax;
                if (0.0 != self->sumb) self->inBuff[i] = self->outMax/self->sumb*self->suma;
	    }
	}
    }
    
    /*return new output */
    return self->outBuff[0];
}

double ikSlti_getOutput(const ikSlti *self) {
    /*return value */
    return self->outBuff[0];
}

/* @endcond */
