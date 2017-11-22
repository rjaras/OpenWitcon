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

/**
 * @file ikVector.h
 * 
 * @brief Class ikVector interface
 */

#ifndef IKVECTOR_H
#define IKVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @struct ikVector
     * @brief Three-dimensional vector
     * 
     * Instances of this type represent three-dimensional vectors.
     * 
     * @par Public members
     * @li @link c @endlink coordinates
     * 
     * @par Methods
     * @li @link ikVector_add @endlink add two instances together
     * @li @link ikVector_mult @endlink multiply an instace by a scalar
     * @li @link ikVector_dot @endlink calculate dot product of two instances
     * @li @link ikVector_cross @endlink calculate cross product of two instances
     * @li @link ikVector_rotate @endlink rotate instance
     * 
     */
    typedef struct ikVector {
        double c[3]; /**<coordinates*/
    } ikVector;
    
    /**
     * Add two instances together
     * @param u @f$ \vec{u} @f$
     * @param v @f$ \vec{v} @f$
     * @return @f$ \vec{u} + \vec{v} @f$
     */
    ikVector ikVector_add(ikVector u, ikVector v);
    
    /**
     * Multiply an instance by a scalar
     * @param vector @f$ \vec{u} @f$
     * @param scalar @f$ a @f$
     * @return @f$ a \vec{u} @f$
     */
    ikVector ikVector_mult(ikVector vector, double scalar);
    
    /**
     * Calculate the dot product of two instances
     * @param u @f$ \vec{u} @f$
     * @param v @f$ \vec{v} @f$
     * @return @f$ \vec{u} \cdot \vec{v} @f$
     */
    double ikVector_dot(ikVector u, ikVector v);
    
    /**
     * Calculate cross product of two instances
     * @param u @f$ \vec{u} @f$
     * @param v @f$ \vec{v} @f$
     * @return @f$ \vec{u} \times \vec{v} @f$
     */
    ikVector ikVector_cross(ikVector u, ikVector v);

    /**
     * Rotate instance by a given angle around a given axis
     * @image html ikVector_rotate.svg
     * @param vector @f$ \vec{u} @f$
     * @param angle @f$ \vec{r} @f$
     * @return @f$ \vec{v} @f$, i.e. @p vector rotated around the direction of @p angle by an angle equal to the magnitude of @p angle, according to the right hand rule
     */
    ikVector ikVector_rotate(ikVector vector, ikVector angle);

#ifdef __cplusplus
}
#endif

#endif /* IKVECTOR_H */

