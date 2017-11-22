/*
Copyright (C) 2017 IK4-IKERLAN

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

#include <math.h>
#include "ikVector.h"

/**
 * @file ikVector.c
 * 
 * @brief Class ikVector implementation
 */

/* @cond */

ikVector ikVector_add(ikVector u, ikVector v) {
    ikVector r;
    int i;

    for (i = 0; i < 3; i++) {
        r.c[i] = u.c[i] + v.c[i];
    }

    return r;
}

ikVector ikVector_mult(ikVector vector, double scalar) {
    ikVector r;
    int i;

    for (i = 0; i < 3; i++) {
        r.c[i] = scalar * vector.c[i];
    }

    return r;
}

double ikVector_dot(ikVector u, ikVector v) {
    double r = 0;
    int i;
    
    for (i = 0; i < 3; i++) {
        r += u.c[i] * v.c[i];
    }
    
    return r;
}

ikVector ikVector_cross(ikVector u, ikVector v) {
    ikVector r;
    
    r.c[0] = u.c[1] * v.c[2] - u.c[2] * v.c[1];
    r.c[1] = u.c[2] * v.c[0] - u.c[0] * v.c[2];
    r.c[2] = u.c[0] * v.c[1] - u.c[1] * v.c[0];
    
    return r;
}

ikVector ikVector_rotate(ikVector vector, ikVector angle) {
    ikVector r;
    ikVector k;
    float a;
    int i;
    
    k.c[0] = 1.0;
    for (i = 1; i < 3; i++) {
        k.c[i] = 0.0;
    }
    
    a = sqrt(ikVector_dot(angle, angle));
    if ( 0 < a ) k = ikVector_mult(angle, 1.0/a);
    
    r = ikVector_cross(k, vector);
    r = ikVector_mult( r, sin(a) );
    r = ikVector_add( r, ikVector_mult(vector, cos(a)) );
    r = ikVector_add( r, ikVector_mult( k, ikVector_dot(k, vector) * (1 - cos(a)) ) );
    
    return r;
}

/* @endcond */
