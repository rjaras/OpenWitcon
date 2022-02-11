/*
Copyright (C) 2021-2022 IKERLAN

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
 * @file ikThrustLim.h
 * 
 * @brief Class ikThrustLim interface
 */

#ifndef IKTHRUSTLIM_H
#define IKTHRUSTLIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ikSurf.h"

    /**
     * @struct ikThrustLim
     * @brief Thrust limiter
     * 
     * Instances of this type are thrust limiters. They take the
     * rotor speed, the tip-speed ratio and a maximum thrust command,
     * and generate a minimum pitch angle.
     * 
     * @par Inputs
     * @li rotor speed in rad/s
     * @li maximum thrust in kN
     * @li tip-speed ratio
     * 
     * @par Outputs
     * @li minimum pitch in degrees
     * 
     * @par Unit block
     * 
     * @image html ikThrustLim_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikThrustLim_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikThrustLim_new @endlink construct new instance
     * @li @link ikThrustLim_eval @endlink evaluate minimum pitch
     * @li @link ikThrustLim_delete @endlink delete instance
     */
    typedef struct ikThrustLim {
	double rho; /**<air density in kg/m^3*/
	double R; /**<rotor radius in m*/
        /**
         * Private members
         */
        /* @cond */
	double rotorSpeed;
	double maximumThrust;
	double tipSpeedRatio;
	double ctlambda2;
	double minimumPitch;
	ikSurf *surfCtlambda2;  /**<surface giving Ct/lambda^2 as a function of lambda and beta*/
        /* @endcond */
    } ikThrustLim;
    
    /**
     * @struct ikThrustLimParams
     * @brief Thrust limiter initialisation parameters
     */
    typedef struct ikThrustLimParams {
	double rho; /**<air density in kg/m^3*/
	double R; /**<rotor radius in m*/
	const char *ctlambda2SurfaceFileName; /**<name of a valid file for @link ikSurf_newf @endlink*/
    } ikThrustLimParams;
    
    /**
     * Initialise an instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li -1: Ctlambda2 surface initialisation error
     */
    int ikThrustLim_init(ikThrustLim *self, const ikThrustLimParams *params);
    
    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikThrustLim_initParams(ikThrustLimParams *params);
    
    /**
     * Execute periodic calculations
     * @param self thrust limiter instance
     * @param tipSpeedRatio tip-speed ratio
     * @param rotorSpeed rotor speed in rad/s
     * @param maximumThrust maximum thrust in kN
     * @return minimum pitch
     */
    double ikThrustLim_step(ikThrustLim *self, double tipSpeedRatio, double rotorSpeed, double maximumThrust);
    
    /**
     * Get output value by name. All signals named on the block diagram of
     * @link ikTsrEst @endlink are accessible.
     *
     * @param self thrust limiter instance
     * @param output output value
     * @param name output name
     * @return error code:
     * @li 0: no error
     * @li -1: invalid signal name
     */
    int ikThrustLim_getOutput(const ikThrustLim *self, double *output, const char *name);

    /**
     * Delete instance
     * @param self thrust limiter instance
     */
    void ikThrustLim_delete(ikThrustLim *self);


#ifdef __cplusplus
}
#endif

#endif /* IKTHRUSTLIM_H */

