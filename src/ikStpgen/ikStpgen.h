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
 * @file ikStpgen.h
 * 
 * @brief Class ikStpgen interface
 */

#ifndef IKSTPGEN_H
#define IKSTPGEN_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ikLutbl.h"
#include "ikLinCon.h"

#define IKSTPGEN_NZONEMAX 8
    
    /**
     * @struct ikStpgen
     * @brief Setpoint generator
     * 
     * Instances of this type are setpoints generators of the sort used in variable
     * wind turbine control. They take the control action of a closed loop controller,
     * such as the torque demand in a below-rated wind turbine controller, and the
     * feedback signal of said closed loop controller, such as the generator speed,
     * and generate the setpoint for said closed loop controller, such as the demanded
     * generator speed, and the control action limits, such as the maximum and minimum
     * torque demand values.
     * 
     * @par Inputs
     * @li external maximum control action, specify via @link ikStpgen_step @endlink
     * @li external minimum control action, specify via @link ikStpgen_step @endlink
     * @li control action, specify via @link ikStpgen_step @endlink
     * @li external maximum setpoint, specify via @link ikStpgen_step @endlink
     * @li feedback, specify via @link ikStpgen_step @endlink
     * 
     * @par Outputs
     * @li setpoint, returned by @link ikStpgen_step @endlink
     * @li maximum control action, get via @link ikStpgen_getOutput @endlink
     * @li minimum control action, get via @link ikStpgen_getOutput @endlink
     * 
     * @par Unit block
     * 
     * @image html ikStpgen_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikStpgen_block_diagram.svg
     * 
     * @par State machine
     * 
     * @image html ikStpgen_state_machine.svg
     * 
     * @par Methods
     * @li @link ikStpgen_initParams @endlink initialise initialisation parameter structure
     * @li @link ikStpgen_init @endlink initialise an instance
     * @li @link ikStpgen_step @endlink execute periodic calculations
     * @li @link ikStpgen_getOutput @endlink get output value
     */
    typedef struct ikStpgen {
        /**
         * Private members
         */
        /* @cond */
        ikLutbl     uopt;
        ikLinCon    yFilters;
        ikLinCon    uFilters;
        int         state;
        int         olsign;
        int         nzones;
        double      setpoints [2][IKSTPGEN_NZONEMAX];
        int         nZoneTransitionSteps [IKSTPGEN_NZONEMAX - 1];
        int         iZoneTransitionSteps [IKSTPGEN_NZONEMAX - 1];
        int         nZoneTransitionLockSteps [IKSTPGEN_NZONEMAX - 1];
        int         iZoneTransitionLockSteps [IKSTPGEN_NZONEMAX - 1];
        int         zoneTransitionLocked [IKSTPGEN_NZONEMAX - 1];
        double      controlActionLimitRate;
        double      r;
        double      minCon;
        double      maxCon;
        double      feedback;
        double      controlAction;
        double      externalMaximumControlAction;
        double      externalMinimumControlAction;
        double      externalMaximumSetpoint;
        /* @endcond */
    } ikStpgen;
    
    /**
     * @struct ikStpgenParams
     * @brief Setpoint generator initialisation parameters
     */
    typedef struct ikStpgenParams {
        ikLinConParams  feedbackFilters;                                        /**<feedback filtering initialisation parameters*/
        ikLinConParams  controlActionFilters;                                   /**<control action filtering initialisation parameters*/
        int             openLoopGainSign;                                       /**<sign of the open loop gain of the feedback loop for which the
                                                                                 setpoint is generated. 0 is valid and will be considered positive.
                                                                                 The default value is -1.*/
        int             preferredControlActionLutblN;                           /**<number of points in the preferred control action curve.
                                                                                 The default is 2.*/
        double          preferredControlActionLutblX    [IKLUTBL_MAXPOINTS];    /**<input values of the points in the preferred control action curve.
                                                                                 The default is {-1.0, 1.0}*/
        double          preferredControlActionLutblY    [IKLUTBL_MAXPOINTS];    /**<output values of the points in the preferred control action curve.
                                                                                 The default is {-1.0, 1.0}*/
        int             nzones;                                                 /**<number of zones where the preferred control action is applicable.
                                                                                 The default is 0.*/
        double          setpoints                       [2][IKSTPGEN_NZONEMAX]; /**<allowed setpoint values, arranged in one column per zone and two rows.
                                                                                The first row contains the lower limit of each zone.
                                                                                The second row contains the upper limit of each zone.
                                                                                 The default is a zero-filled array.*/
        int             nZoneTransitionSteps            [IKSTPGEN_NZONEMAX - 1];/**<number of steps or sampling intervals the setpoint takes to go from
                                                                                 * the upper limit of a zone to the lower limit of the next, and vice versa.
                                                                                 * The default is 0.*/
        int             nZoneTransitionLockSteps        [IKSTPGEN_NZONEMAX - 1];/**<number of steps or sampling intervals a zone transition is locked
                                                                                 * after it happened once. If zoneTransitionPrelock is 1, zone transitions
                                                                                 * are locked from initialisation.
                                                                                 * The default is 0.*/
        int             zoneTransitionPrelock;                                  /**<when 1, zone transitions are locked from initialisation and for the number
                                                                                 * of steps specified by nZoneTransitionLockSteps. When 0, zone transitions
                                                                                 * are locked only after they happen once. The default is 1.*/
        double          controlActionLimitRate;                                 /**<rate at which control action limits vary. If 0, control action limits
                                                                                 change abruptly. The default is 0.*/
    } ikStpgenParams;
    
    /**
     * Initialise an instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li -1: could not initialise feedback filtering
     * @li -2: could not initialise control action filtering
     * @li -3: could not initialise preferred control action curve
     * @li -4: invalid zone number, it must be between 0 and @link IKSTPGEN_NZONEMAX @endlink
     * @li -5: invalid setpoints, they must be sorted
     * @li -6: invalid zone transition step number, it must be non-negative
     * @li -7: invalid zone transition lock step number, it must be non-negative
     * @li -8: invalid control action limit rate, it must be non-negative
     * @li -9: invalid zone transition pre-lock value, it must be 0 or 1
     */
    int ikStpgen_init(ikStpgen *self, const ikStpgenParams *params);
    
    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikStpgen_initParams(ikStpgenParams *params);
    
    /**
     * Execute periodic calculations
     * @param self setpoint generator instance
     * @param maxSp external maximum setpoint
     * @param feedback feedback
     * @param conAct control action
     * @param minCon external minimum control action
     * @param maxCon external maximum control action
     * @return setpoint
     */
    double ikStpgen_step(ikStpgen *self, double maxSp, double feedback, double conAct, double minCon, double maxCon);
    
    /**
     * Get output value by name. All signals named on the block diagram of
     * @link ikStpgen @endlink are accessible.
     * @param self setpoint generator instance
     * @param output output value
     * @param name output name
     * @return error code:
     * @li 0: no error
     * @li -1: invalid signal name
     * @li -2: invalid block name
     */
    int ikStpgen_getOutput(const ikStpgen *self, double *output, const char *name);


#ifdef __cplusplus
}
#endif

#endif /* IKSTPGEN_H */

