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
 * @file ikTfList.h
 * 
 * @brief Class ikTfList interface
 */

#ifndef IKTFLIST_H
#define IKTFLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ikSlti.h"

#define IKTFLIST_NMAX 8

    /**
     * @struct ikTfParams
     * @brief transfer function initialisation parameters
     */
    typedef struct ikTfParams {
        int     enable;     /**<flag: 0 to disable the transfer function,
                             any other value to enable it. When disabled, the
                             transfer function will function normally, but be bypassed.
                             The default value is 0.*/
        int     *variableEnable;    /**<pointer to a persistent address where a flag
                                    is stored for the enable flag. If not NULL,
                                    the value at this address will override
                                    @link enable @endlink at every time step.
                                     The default value is NULL.*/
        double  a[3];       /**<denominator parameters, as in @link ikSlti_setParam @endlink.
                             The default value is {1, 0, 0}.*/
        double  b[3];       /**<numerator parameters, as in @link ikSlti_setParam @endlink
                             The default value is {1, 0, 0}.*/
        double  *maxInput;  /**<pointer to a persistent address where a value
                             is stored for the upper boundary of the transfer
                             function input. If NULL, the upper boundary of the
                             transfer function input will be disabled.
                             The default value is 0.*/
        double  *minInput;  /**<pointer to a persistent address where a value
                             is stored for the lower boundary of the transfer
                             function input. If NULL, the lower boundary of the
                             transfer function input will be disabled.
                             The default value is 0.*/
        double  *maxOutput; /**<pointer to a persistent address where a value
                             is stored for the upper boundary of the transfer
                             function output. If NULL, the upper boundary of the
                             transfer function output will be disabled.
                             The default value is 0.*/
        double  *minOutput; /**<pointer to a persistent address where a value
                             is stored for the lower boundary of the transfer
                             function output. If NULL, the lower boundary of the
                             transfer function output will be disabled.
                             The default value is 0.*/
    } ikTfParams;
    
    /**
     * @struct ikTfListParams
     * @brief transfer function list initialisation parameters
     */
    typedef struct ikTfListParams {
        ikTfParams tfParams [IKTFLIST_NMAX];    /**<initialisation parameters for each individual transfer function*/
    } ikTfListParams;
    
    /**
     * @struct ikTfList
     * @brief Transfer function list
     * 
     * Instances of this type are lists of transfer functions to be applied in
     * series to a signal.
     * 
     * @par Inputs
     * @li input: specify via @link ikTfList_step @endlink
     * 
     * @par Outputs
     * @li output: returned by @link ikTfList_step @endlink
     * 
     * @par Unit block
     * 
     * @image html ikTfList_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikTfList_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikTfList_init @endlink initialise an instance
     * @li @link ikTfList_step @endlink execute periodic calculations
     * @li @link ikTfList_getOutput @endlink get output value
     */
    typedef struct ikTfList {
        /**
         * Private members
         */
        /* @cond */
        ikSlti  tfs          [IKTFLIST_NMAX];
        int     enable       [IKTFLIST_NMAX];
        int     *varEnable   [IKTFLIST_NMAX];
        double  *maxInput    [IKTFLIST_NMAX];
        double  *minInput    [IKTFLIST_NMAX];
        double  *maxOutput   [IKTFLIST_NMAX];
        double  *minOutput   [IKTFLIST_NMAX];
        /* @endcond */
    } ikTfList;

    /**
     * Initialise instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li -x: could not initialise x-th transfer function (starting at 1)
     */
    int ikTfList_init(ikTfList *self, const ikTfListParams *params);
    
    /**
     * Initialise initialisation parameter structure with default values
     * @param params initialisation parameter structure
     */
    void ikTfList_initParams(ikTfListParams *params);
    
    /**
     * Execute periodic calculations
     * @param self transfer function list instance
     * @param input new input value
     * @return new output value
     */
    double ikTfList_step(ikTfList *self, double input);
    
    /**
     * Get output value
     * @param self transfer function list instance
     * @param index index of the transfer function whose output is to be returned,
     * with 0 for the last to be applied, 1 for the last but one to be applied,
     * and so on. Values below 0 and above @link IKTFLIST_NMAX @endlink - 1 are valid
     * and equivalent to 0 and @link IKTFLIST_NMAX @endlink - 1, respectively.
     * @return output value
     */
    double ikTfList_getOutput(const ikTfList *self, int index);

#ifdef __cplusplus
}
#endif

#endif /* IKTFLIST_H */

