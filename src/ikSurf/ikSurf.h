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
 * @file ikSurf.h
 *
 * @brief Class ikSurf interface
 */

#include <stdlib.h>

#ifndef IKSURF_H
#define IKSURF_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @struct ikSurf
     * @brief surface
     *
     * Instances of this class are implementations of n-dimensional surfaces.
     * Linear interpolation and extrapolation are applied.
     *
     * @par Inputs
     * @li point of evaluation, specify via @link ikSurf_eval @endlink
     *
     * @par outputs
     * @li evaluation result, returned by @link ikSurf_eval @endlink
     *
     * @par Unit block
     *
     * @image html ikSurf_unit_block.svg
     *
     * @par Block diagram
     *
     * @image html ikSurf_block_diagram.svg
     *
     * @par Methods
     * @li @link ikSurf_new @endlink get new instance
     * @li @link ikSurf_newf @endlink get new instance from file
     * @li @link ikSurf_clone @endlink clone instance
     * @li @link ikSurf_delete @endlink delete instance
     * @li @link ikSurf_getDimensions @endlink get number of dimensions
     * @li @link ikSurf_getPointNumber @endlink get number of data points per dimension
     * @li @link ikSurf_eval @endlink evaluate surface coordinate
     */
    typedef struct ikSurf ikSurf;
    struct ikSurf {
        /**
         * Private members
         */
        /* @cond */
        int linked; /*flag indicating whether the data ought to be deleted on delete*/
        int dims; /*number of dimensions */
        int * ndata; /*number of data points per dimension*/
        double * data; /*data points */
        double ** coord; /*pointers to coordinate values*/
        ikSurf ** ext; /*extreme surfaces*/
        int ** extidx; /*extreme surface indices*/
        int extidxmax, extidxmin; /*maximum and minimum extreme surface indices*/
        int ** idx; /*coordinate indices, lower and upper bounds*/
        int * idx_i; /*indices to choose from lower bounds, midpoints, upper bounds for each coordinate index*/
        double * interp; /*interpolated values*/
		long interpNumel; /*number of interpolated values*/
        int interp_i; /*index to go down the interpolated values*/
        double * x; /*evaluation coordinates*/
        double * xaux; /*auxiliary evaluation coordinates*/
        int interpOnly; /*flag indicating that eval should only return non-zero for last dimension*/
        /* @endcond */
    };

    /**
     * get new instance
     * @param obj new instance
     * @param dims number of dimensions
     * @param ndata number of data points per dimension (always 1 for the last dimension, which may be omitted if copy!=0)
     * @param data data points, specified as follows:
     * @li surface coordinate values (x0) for the first dimension, in ascending order
     * @li surface coordinate values (x1) for the second dimension, in ascending order
     * @li ...
     * @li surface coordinate values (y) for the last dimension, in the following order:
     *     for i = number of different x0 values
     *         for j = number of different x1 values
     *             for k = number of different x2 values
     *                 y( x0_i, x1_j, x2_k, ... )
     * @param copy copy the input data if !=0, link otherwise
     * @return error message
     * @li "": no error
     * @li "bad dims": dims is non-positive (*obj is set to NULL)
     * @li "bad ndata": ndata is non-positive (*obj is set to NULL)
     * @li "bad data (not ascending)": coordinate values for dimension other than last not in ascending order (*obj is set to NULL)
     * @li "bad data (neither concave nor convex)": coordinate values for last dimension neither concave nor convex (an instance is still passed via *obj, but @link ikSurf_eval @endlink will only work for the last dimension; it will return 0.0 for other dimensions)
     */
    const char * ikSurf_new(ikSurf **obj, int dims, int *ndata, double *data, int copy);

    /**
     * get new instance from file
     * @param obj new instance
     * @param filename path to binary file containing:
     * @li number of dimensions, int32
     * @li number of data points per dimension, int32 (always 1 for the last dimension, which is omitted)
     * @li data points, specified as in @link ikSurf_new @endlink, float64
     * @return error message
     * @li "bad file <filename>": could not open file
     * @li others: see @link ikSurf_new @endlink
     */
    const char * ikSurf_newf(ikSurf **obj, const char *filename);

    /**
     * get clone of existing instance, either copying or linking its data
     * @param obj new instance
     * @param origin instance to be cloned
     * @param copy copy the data of the original instance if !=0, link otherwise
     * @return error message
     * @li "": no error
     */
    const char * ikSurf_clone(ikSurf **obj, const ikSurf *origin, int copy);

    /**
     * delete instance
     * @param self instance
     */
    void ikSurf_delete(ikSurf *self);

    /**
     * get number of dimensions
     * @param self instance
     * @return number of dimensions
     */
    int ikSurf_getDimensions(const ikSurf *self);

    /**
     * get number of data points per dimension
     * @param self instance
     * @param dim dimension (starting at 0)
     * @return number of different coordinate values in data point set for dimension dim (if dim is not within the valid range, return 0)
     */
    int ikSurf_getPointNumber(const ikSurf *self, int dim);

    /**
     * evaluate surface coordinate for dimension dim
     * @param self instance
     * @param dim dimension corresponding to unknown coordinate (staring at 0)
     * @param x known coordinate values, in ascending order of dimension
     * @param side of the extreme to stay on if the surface is convex or concave at x:
     * @li 1: stay at or above extreme
     * @li -1: stay at or below extreme
     * @li otherwise: ignore
     * @return coordinate value for dimension dim (or 0.0 if dim is not valid)
     */
    double ikSurf_eval(ikSurf *self, int dim, const double x[], int side);


#ifdef __cplusplus
}
#endif

#endif /* IKSURF_H */
