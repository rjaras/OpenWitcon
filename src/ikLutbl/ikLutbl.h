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
 * @file ikLutbl.h
 * 
 * @brief Class ikLutbl interface
 */

#ifndef IKLUTBL_H
#define IKLUTBL_H

#ifdef __cplusplus
extern "C" {
#endif

#define IKLUTBL_MAXPOINTS 256
    
    /**
     * @struct ikLutbl
     * @brief look-up table
     * 
     * Instances of this class are implementations of single input single output
     * look-up tables. Linear interpolation and extrapolation are applied.
     * 
     * @par Inputs
     * @li point of evaluation, specify via @link ikLutbl_eval @endlink
     * 
     * @par outputs
     * @li evaluation result, returned by @link ikLutbl_eval @endlink
     * 
     * @par Unit block
     * 
     * @image html ikLutbl_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikLutbl_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikLutbl_init @endlink initialise instance
     * @li @link ikLutbl_getPointNumber @endlink get number of points in the look-up table
     * @li @link ikLutbl_setPoints @endlink set points which define the look-up table
     * @li @link ikLutbl_getPoints @endlink get points which define look-up table
     * @li @link ikLutbl_eval @endlink evaluate look-up table output
     */
    typedef struct ikLutbl {
        /**
         * Private members
         */
        /* @cond */
        int m; /*number of points specifying the look-up table */
        double x[IKLUTBL_MAXPOINTS]; /*input value for each of the points */
        double y[IKLUTBL_MAXPOINTS]; /*output value for each of the points */
        /* @endcond */
    } ikLutbl;

    /**
     * initialise instance
     * @param self instance
     */
    void ikLutbl_init(ikLutbl *self);

    /**
     * get number of points in the look-up table
     * @param self instance
     * @return number of points
     */
    int ikLutbl_getPointNumber(ikLutbl *self);

    /**
     * set points which define the look-up table
     * @param self instance
     * @param m length of arrays at x and y, must be equal to or less than @link IKLUTBL_MAXPOINTS @endlink
     * @param x keys, must be sorted and unique
     * @param y values
     * @return error code
     * @li 0: no error
     * @li -1: invalid array length, must be larger than 0 and equal to or less than @link IKLUTBL_MAXPOINTS @endlink
     * @li -2: keys are not sorted
     */
    int ikLutbl_setPoints(ikLutbl *self, int m, const double x[], const double y[]);

    /**
     * get points which define look-up table
     * @param self instance
     * @param m length of arrays at x and y, must be equal to or less than @link IKLUTBL_MAXPOINTS @endlink
     * @param x keys
     * @param y values
     * @return error code
     * @li 0: no error
     * @li -1: invalid array length, must be equal to or less than @link IKLUTBL_MAXPOINTS @endlink
     */
    int ikLutbl_getPoints(const ikLutbl *self, int m, double x[], double y[]);

    /**
     * evaluate look-up table output
     * @param self instance
     * @param x input value
     * @return output value
     */
    double ikLutbl_eval(const ikLutbl *self, double x);


#ifdef __cplusplus
}
#endif

#endif /* IKLUTBL_H */

