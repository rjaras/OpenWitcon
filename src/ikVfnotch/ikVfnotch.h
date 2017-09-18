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
 * @file ikVfnotch.h
 * 
 * @brief Class ikVfnotch interface
 */

#ifndef IKVFNOTCH_H
#define IKVFNOTCH_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include "ikSlti.h"

    /**
     * @struct ikVfnotch
     * @brief variable frequency notch filter
     * 
     * This class implements 2nd order notch filters.
     * 
     * @par Inputs
     * @li input: signal to be filtered, specify via @link ikVfnotch_step @endlink
     * 
     * @par Outputs
     * @li output: filtered signal, returned by @link ikVfnotch_step @endlink
     * 
     * @par Unit block
     * 
     * @image html ikVfnotch_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikVfnotch_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikVfnotch_init @endlink
     * @li @link ikVfnotch_setSamplingTime @endlink
     * @li @link ikVfnotch_getSamplingTime @endlink
     * @li @link ikVfnotch_setFreq @endlink
     * @li @link ikVfnotch_getFreq @endlink
     * @li @link ikVfnotch_setDamp @endlink
     * @li @link ikVfnotch_getDamp @endlink
     * @li @link ikVfnotch_step @endlink
     * @li @link ikVfnotch_getOutput @endlink
     */
    typedef struct ikVfnotch {
        /**
         * Private members
         */
        /* @cond */
        ikSlti filter; /*discrete-time implementation */
        double dT;
        double freq;
        double dampDen;
        double dampNum;
        /* @endcond */
    } ikVfnotch;

    /**
     * Initialise instance.
     * 
     * The instance is initialised as a discrete-time implementation of a filter
     * with the following transfer function:
     * 
     * @cond
     *         s^2 + 2*dampNum*freq*s + freq^2
     * G(s) = ---------------------------------
     *         s^2 + 2*dampDen*freq*s + freq^2
     * @endcond
     * 
     * @f[
     *  G(s) = \frac{s^2 + 2 \delta_N \omega s + \omega^2}{s^2 + 2 \delta_D \omega s + \omega^2}
     * @f]
     * 
     * The discretisation is carried out via Tustin's approximation:
     * 
     * @cond
     *     2 (z - 1)
     * s = - -------
     *     T (z + 1)
     * @endcond
     * 
     * @f[
     *  s = \frac{2}{T} \frac{z-1}{z+1}
     * @f]
     * 
     * @param self instance
     * @param dT sampling time, @f$T@f$ [s]
     * @param freq frequency, @f$\omega@f$ [rad/s]
     * @param dampDen denominator damping coefficient, @f$\delta_D@f$
     * @param dampNum numerator damping coefficient, @f$\delta_N@f$
     * @return error code:
     * @li 0: no error
     * @li -1: invalid sampling time, must be positive
     * @li -2: invalid frequency, must be positive
     */
    int ikVfnotch_init(ikVfnotch *self, double dT, double freq, double dampDen, double dampNum);
    
    /**
     * Set instance sampling time.
     * @param self instance
     * @param dT sampling time, @f$T@f$ [s]
     * @return error code:
     * @li 0: no error
     * @li -1: invalid sampling time, must be positive
     */
    int ikVfnotch_setSamplingTime(ikVfnotch *self, double dT);
    
    /**
     * Get instance sampling time.
     * @param self instance
     * @return sampling time, @f$T@f$ [s]
     */
    double ikVfnotch_getSamplingTime(const ikVfnotch *self);
    
    /**
     * Set instance frequency.
     * @param self instance
     * @param freq frequency, @f$\omega@f$ [rad/s]
     * @return error code:
     * @li 0: no error
     * @li -2: invalid frequency, must be positive
     */
    int ikVfnotch_setFreq(ikVfnotch *self, double freq);
    
    /**
     * Get instance frequency.
     * @param self instance
     * @return frequency, @f$\omega@f$ [rad/s]
     */
    double ikVfnotch_getFreq(const ikVfnotch *self);
    
    /**
     * Set instance damping coefficients.
     * @param self instance
     * @param dampDen denominator damping coefficient, @f$\delta_D@f$
     * @param dampNum numerator damping coefficient, @f$\delta_N@f$
     */
    void ikVfnotch_setDamp(ikVfnotch *self, double dampDen, double dampNum);
    
    /**
     * Get instance damping coefficients.
     * @param self instance
     * @param dampDen denominator damping coefficient, @f$\delta_D@f$
     * @param dampNum numerator damping coefficient, @f$\delta_N@f$
     */
    void ikVfnotch_getDamp(const ikVfnotch *self, double *dampDen, double *dampNum);
    
    /**
     * Advance one sampling interval and calculate filter output
     * @param self instance
     * @param input input value
     * @return output value
     */
    double ikVfnotch_step(ikVfnotch *self, double input);
    
    /**
     * Get filter output.
     * @param self instance
     * @return output value
     */
    double ikVfnotch_getOutput(const ikVfnotch *self);


#ifdef __cplusplus
}
#endif

#endif /* IKVFNOTCH_H */

