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
 * @file ikNotchList.h
 * 
 * @brief Class ikNotchList interface
 */

#ifndef IKNOTCHLIST_H
#define IKNOTCHLIST_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ikVfnotch.h"
    
#define IKNOTCHLIST_NMAX 8
    
    /**
     * @struct ikNotchParams
     * @brief Notch filter initialisation parameters
     */
    typedef struct ikNotchParams {
        int     enable;             /**<flag: 0 to disable the notch filter,
                                    any other value to enable it. When disabled, the
                                    notch filter will function normally, but be bypassed.
                                    The default value is 0;*/
        int     *variableEnable;    /**<pointer to a persistent address where a flag
                                    is stored for the enable flag. If not NULL,
                                    the value at this address will override
                                    @link enable @endlink at every time step.
                                    The default value is NULL.*/
        double  freq;               /**<frequency, @f$\omega@f$ [rad/s], as in @link ikVfnotch_init @endlink.
                                    The default value is 1.0.*/
        double  *variableFreq;      /**<pointer to a persistent address where a
                                    value is stored for the filter frequency. If
                                    not NULL, the value at this address will override
                                    @link freq @endlink at every time step.
                                    The default value is NULL.*/
        double  dampDen;            /**<denominator damping coefficient, @f$\delta_D@f$, as in @link ikVfnotch_init @endlink.
                                    The default value is 1.0.*/
        double  dampNum;            /**<numerator damping coefficient, @f$\delta_N@f$, as in @link ikVfnotch_init @endlink.
                                    The default value is 1.0.*/
    } ikNotchParams;
    
    /**
     * @struct ikNotchListParams
     * @brief Notch filter list initialisation parameters
     */
    typedef struct ikNotchListParams {
        double         dT;                                  /**<sampling time, @f$T@f$ [s], as in @link ikVfnotch_init @endlink*/
        ikNotchParams  notchParams     [IKNOTCHLIST_NMAX];  /**<initialisation parameters for every individual notch filter*/
    } ikNotchListParams;

    /**
     * @struct ikNotchList
     * @brief Notch filter list
     * 
     * Instances of this type are lists of transfer functions which are applied
     * in series to a signal.
     * 
     * @par Inputs
     * @li input: specify via @link ikNotchList_step @endlink
     * 
     * @par Outputs
     * @li output: returned by @link ikNotchList_step @endlink
     * 
     * @par Unit block
     * 
     * @image html ikNotchList_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikNotchList_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikNotchList_init @endlink initialise an instance
     * @li @link ikNotchList_step @endlink execute periodic calculations
     * @li @link ikNotchList_getOutput @endlink get output value
     */
    typedef struct ikNotchList {
        /**
         * Private members
         */
        /* @cond */
        ikVfnotch   notches         [IKNOTCHLIST_NMAX];
        double      *variableFreq   [IKNOTCHLIST_NMAX];
        int         enable          [IKNOTCHLIST_NMAX];
        int         *variableEnable [IKNOTCHLIST_NMAX];
        /* @endcond */
    } ikNotchList;

    /**
     * Initialise instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li 1: invalid sampling time
     * @li -x: could not initialise x-th notch filter
     */
    int ikNotchList_init(ikNotchList *self, const struct ikNotchListParams *params);
    
    /**
     * Initialise initialisation parameter structure with default values
     * @param params initialisation parameter structure
     */
    void ikNotchList_initParams(struct ikNotchListParams *params);
    
    /**
     * Execute periodic calculations
     * @param self notch filter lits instance
     * @param input new input value
     * @return new output value
     */
    double ikNotchList_step(ikNotchList *self, double input);
    
    /**
     * Get output value
     * @param self notch filter list instance
     * @param index index of the notch filter whose output is to be returned,
     * with 0 for the last to be applied, 1 for the last but one to be applied,
     * and so on. Values below 0 and above @link IKNOTCHLIST_NMAX @endlink - 1 are valid
     * and equivalent to 0 and @link IKNOTCHLIST_NMAX @endlink - 1, respectively.
     * @return output value
     */
    double ikNotchList_getOutput(const ikNotchList *self, int index);
    

#ifdef __cplusplus
}
#endif

#endif /* IKNOTCHLIST_H */

