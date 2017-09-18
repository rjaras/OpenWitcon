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
 * @file ikConLoop.h
 * 
 * @brief Class ikConLoop interface
 */

#ifndef IKCONLOOP_H
#define IKCONLOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ikLinCon.h"
#include "ikStpgen.h"
#include "ikRegionSelector.h"

    /**
     * @struct ikConLoop
     * @brief Control loop
     * 
     * Instances of this type are wind turbine feedback control loops.
     * 
     * @par Inputs
     * @li maximum setpoint value, specify via @link ikConLoop_step @endlink
     * @li feedback value, specify via @link ikConLoop_step @endlink
     * @li external maximum control action, specify via @link ikConLoop_step @endlink
     * @li external minimum control action, specify via @link ikConLoop_step @endlink
     * 
     * @par Outputs
     * @li control action, returned by @link ikConLoop_step @endlink
     * 
     * @par Unit block
     * 
     * @image html ikConLoop_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikConLoop_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikConLoop_initParams @endlink initialise initialisation parameter structure
     * @li @link ikConLoop_init @endlink initialise an instance
     * @li @link ikConLoop_step @endlink execute preriodic calculations
     * @li @link ikConLoop_getOutput @endlink get output value
     */
    typedef struct ikConLoop {
        /**
         * Private members
         */
        /* @cond */
        ikLinCon            setpointFilters;
        ikLinCon            controlActionFilters;
        ikRegionSelector    regionSelector;
        int                 selectedRegion;
        ikLinCon            lincon;
        ikStpgen            stpgen;
        double              maximumControlAction;
        double              minimumControlAction;
        double              controlAction;
        double              x;
        double              y;
        /* @endcond */
    } ikConLoop;
    
    /**
     * @struct ikConLoopParams
     * @brief Control loop initialisation parameters
     */
    typedef struct ikConLoopParams {
        ikLinConParams          linearController;   /**<linear controller initialisation parameters.
                                             The last transfer function applied on the error
                                             signal will be enabled at initialisation regardless
                                             of the enable value provided here, and its saturation
                                             limits will be overridden. Online modification
                                             of the enable setting and all other parameters
                                             will be respected.*/
        ikStpgenParams          setpointGenerator;   /**<setpoint generator initialisation parameters*/
        ikRegionSelectorParams  regionSelector; /**<region selector initialisation parameters.*/
        ikLinConParams          setpointFilters; /**<setpoint filter initialisation parameters*/
        ikLinConParams          controlActionFilters; /**<control action filter initialisation parameters*/
    } ikConLoopParams;
    
    /**
     * Initialise an instance
     * @param self instance
     * @param params initialisation parameters. The last transfer function applied
     * by the control loop will be enabled and its saturation limits overridden.
     * @return error code:
     * @li 0: no error
     * @li -1: could not initialise setpoint generator
     * @li -2: could not initialise linear controller
     * @li -3: could not initialise setpoint filters
     * @li -4: could not initialise control action filters
     * @li -5: could not initialise preset selector
     */
    int ikConLoop_init(ikConLoop *self, const ikConLoopParams *params);
    
    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikConLoop_initParams(ikConLoopParams *params);
    
    /**
     * Execute periodic calculations
     * @param self control loop instance
     * @param maxSp external maximum setpoint
     * @param feedback feedback
     * @param minCon external minimum control action
     * @param maxCon external maximum control action
     * @return control action value
     */
    double ikConLoop_step(ikConLoop *self, double maxSp, double feedback, double minCon, double maxCon);
    
    /**
     * Get output value by name. All signals named on the block diagram of
     * @link ikStpgen @endlink are accessible.
     * @param self control loop instance
     * @param output output value
     * @param name output name
     * @return error code:
     * @li 0: no error
     */
    int ikConLoop_getOutput(const ikConLoop *self, double *output, const char *name);


#ifdef __cplusplus
}
#endif

#endif /* IKCONLOOP_H */

