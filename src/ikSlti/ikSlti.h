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
 * @file ikSlti.h
 * 
 * @brief Class ikSlti interface
 */

#ifndef IKSLTI_H
#define IKSLTI_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @struct ikSlti
     * @brief Saturating linear time invariant system
     * 
     * Instances of this type are discrete-time implementations of 2nd order
     * linear time invariant single input single output systems with saturation
     * limits applied on input and output values.
     * 
     * @par Inputs
     * @li input value: set via @link ikSlti_step @endlink
     * @li input saturation: set via @link ikSlti_setInSat @endlink
     * @li output saturation: set via @link ikSlti_setOutSat @endlink
     * 
     * @par Outputs
     * @li output value: returned by @link ikSlti_step @endlink and @link ikSlti_getOutput @endlink
     * 
     * @par Unit block
     * 
     * @image html ikSlti_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikSlti_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikSlti_init @endlink initialise an instance
     * @li @link ikSlti_setParam @endlink set parameters
     * @li @link ikSlti_getParam @endlink get parameters
     * @li @link ikSlti_setBuff @endlink set buffers
     * @li @link ikSlti_getBuff @endlink get buffers
     * @li @link ikSlti_setInSat @endlink set input saturation
     * @li @link ikSlti_getInSat @endlink get input saturation
     * @li @link ikSlti_setOutSat @endlink set output saturation
     * @li @link ikSlti_getOutSat @endlink get output saturation
     * @li @link ikSlti_step @endlink execute periodic calculations
     * @li @link ikSlti_getOutput @endlink get output value
     */
    typedef struct ikSlti {
        /**
         * Private members
         */
        /* @cond */
        double inBuff[3]; /*input buffer */
        double outBuff[3]; /*output buffer */
        double a[3]; /*denominator parameters */
        double b[3]; /*numerator parameters */
        double suma; /*sum of a */
        double sumb; /*sum of b */
        int inSat; /*input saturation status flag */
        int outSat; /*output saturation status flag */
        double inMax; /*upper saturation limit for input */
        double inMin; /*lower saturation limit for input */
        double outMax; /*upper saturation limit for output */
        double outMin; /*lower saturation limit for output */
        /* @endcond */
    } ikSlti;
    
    /**
     * initialise instance
     * @param self instance
     */
    void ikSlti_init(ikSlti *self);

    /**
     * set LTI system parameter values
     * 
     * The LTI system is represented by the follwoing discrete-time transfer
     * function:
     * @cond
     *         b[0] + b[1]*z^(-1) + b[2]*z^(-2)
     * G(z) = ---------------------------------
     *         a[0] + a[1]*z^(-1) + a[2]*z^(-2)
     * @endcond
     * 
     * @f[
     *  G(z) = \frac{b_0 + b_1 z^{-1} + b_2 z^{-2}}{a_0 + a_1 z^{-1} + a_2 z^{-2}}
     * @f]
     * 
     * This method sets the values of @f$a_i@f$ and @f$b_i@f$ of an instance.
     * 
     * @param self instance
     * @param a array of length 3 with denominator parameters, where @f$a_i@f$ = a[i] and a[0] must be non-zero
     * @param b array of length 3 with numerator parameters, where @f$b_i@f$ = b[i]
     * @return error code:
     * @li 0: no error
     * @li -1: invalid value at a[0], must be non-zero
     */
    int ikSlti_setParam(ikSlti *self, const double a[], const double b[]);

    /**
     * set buffer values
     * 
     * The LTI system is of order 2, and therefore remembers the last 3 values
     * of its input and its output. This method allows the user to set the
     * values remembered by the instance.
     * 
     * @param self instance
     * @param inBuff array with the input values, in chronological order, with
     * inBuff[0] being the latest and inBuff[2] the oldest
     * @param outBuff array with the output values, in chronological order, with
     * outBuff[0] being the latest and outBuff[2] the oldest
     */
    void ikSlti_setBuff(ikSlti *self, const double inBuff[], const double outBuff[]);

    /**
     * get LTI system parameter values
     * 
     * The LTI system is represented by the follwoing discrete-time transfer
     * function:
     * @cond
     *         b[0] + b[1]*z^(-1) + b[2]*z^(-2)
     * G(z) = ---------------------------------
     *         a[0] + a[1]*z^(-1) + a[2]*z^(-2)
     * @endcond
     * 
     * @f[
     *  G(z) = \frac{b_0 + b_1 z^{-1} + b_2 z^{-2}}{a_0 + a_1 z^{-1} + a_2 z^{-2}}
     * @f]
     * 
     * This method gets the values of @f$a_i@f$ and @f$b_i@f$ of an instance.
     * 
     * @param self instance
     * @param a array of length 3 for denominator parameters, where a[i] = @f$a_i@f$
     * @param b array of length 3 for numerator parameters, where b[i] = @f$b_i@f$
     */
    void ikSlti_getParam(const ikSlti *self, double a[], double b[]);

    /**
     * get buffer values
     * 
     * The LTI system is of order 2, and therefore remembers the last 3 values
     * of its input and its output. This method allows the user to get the
     * values remembered by the instance.
     * 
     * @param self instance
     * @param inBuff array for the input values, in chronological order, with
     * inBuff[0] being the latest and inBuff[2] the oldest
     * @param outBuff array for the output values, in chronological order, with
     * outBuff[0] being the latest and outBuff[2] the oldest
     */
    void ikSlti_getBuff(const ikSlti *self, double inBuff[], double outBuff[]);

    /**
     * set input saturation limits
     * 
     * @param self instance
     * @param enable flag:
     * @li 0 to disable input saturation
     * @li -1 to enable lower saturation limit only
     * @li 1 to enable upper saturation limit only
     * @li 2 to enable both
     * @param min lower saturation limit
     * @param max upper saturation limit
     * @return error code
     * @li 0: no error
     * @li -1: invalid enable flag value, must be -1, 0, 1 or 2
     * @li -2: invalid saturation limits, upper limit must be larger than or equal
     * to lower limit
     */
    int ikSlti_setInSat(ikSlti *self, int enable, double min, double max);

    /**
     * get input saturation limits
     * 
     * @param self instance
     * @param min lower saturation limit
     * @param max upper saturation limit
     * @return flag:
     * @li 0 if input saturation is disabled
     * @li -1 if only the lower saturation limit is enabled
     * @li 1 if only the upper saturation is enabled
     * @li 2 if both are enabled
     */
    int ikSlti_getInSat(const ikSlti *self, double *min, double *max);

    /**
     * set output saturation limits
     * 
     * @param self instance
     * @param enable flag:
     * @li 0 to disable output saturation
     * @li -1 to enable lower saturation limit only
     * @li 1 to enable upper saturation limit only
     * @li 2 to enable both
     * @param min lower saturation limit
     * @param max upper saturation limit
     * @return error code
     * @li 0: no error
     * @li -1: invalid enable flag value, must be -1, 0, 1 or 2
     * @li -2: invalid saturation limits, upper limit must be larger than or equal
     * to lower limit
     */
    int ikSlti_setOutSat(ikSlti *self, int enable, double min, double max);

    /**
     * get output saturation limits
     * 
     * @param self instance
     * @param min lower saturation limit
     * @param max upper saturation limit
     * @return flag:
     * @li 0 if output saturation is disabled
     * @li -1 if only the lower saturation limit is enabled
     * @li 1 if only the upper saturation is enabled
     * @li 2 if both are enabled
     */
    int ikSlti_getOutSat(const ikSlti *self, double *min, double *max);

    /**
     * advance one sample interval and calculate new output of LTI system
     * 
     * @param self instance
     * @param input new input value
     * @return new output value
     */
    double ikSlti_step(ikSlti *self, double input);

    /**
     * get LTI system output
     * 
     * @param self instance
     * @return LTI system output
     */
    double ikSlti_getOutput(const ikSlti *self);


#ifdef __cplusplus
}
#endif

#endif /* IKSLTI_H */

