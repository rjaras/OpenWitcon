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
 * @file ikTsrEst.h
 * 
 * @brief Class ikTsrEst interface
 */

#ifndef IKTSREST_H
#define IKTSREST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ikSurf.h"
#include "ikNotchList.h"
#include "ikTfList.h"

    /**
     * @struct ikTsrEst
     * @brief Tip-speed ratio estimator
     * 
     * Instances of this type are tip-speed ratio estimators. They take the
     * generator speed, the generator torque and the collective pitch angle,
     * and generate an estimated tip-speed ratio and a filtered rotor speed
     * to be used with that estimation for torque and thrust calculations.
     * 
     * @par Inputs
     * @li generator speed in rad/s
     * @li generator torque in kNm
     * @li pitch angle in degrees
     * 
     * @par Outputs
     * @li tip-speed ratio
     * @li rotor speed in rad/s
     * 
     * @par Unit block
     * 
     * @image html ikTsrEst_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikTsrEst_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikTsrEst_initParams @endlink initialise initialisation parameter structure
     * @li @link ikTsrEst_init @endlink initialise an instance
     * @li @link ikTsrEst_step @endlink execute periodic calculations
     * @li @link ikTsrEst_getOutput @endlink get output value
     * @li @link ikTsrEst_delete @endlink delete instance
     */
    typedef struct ikTsrEst {
	double b; /**<gearbox ratio, i.e. high-speed shaft speed divided by low-speed shaft speed*/
	double J; /**<rotor moment of inertia in kgm^2*/
	double rho; /**<air density in kg/m^3*/
	double R; /**<rotor radius in m*/
        /**
         * Private members
         */
        /* @cond */
	double unfilteredRotorSpeed;
	double generatorSpeed;
	double generatorTorque;
	double rotorAcceleration;
	double rotorSpeed;
	double filteredGeneratorTorque;
	double pitchAngle;
	double filteredPitchAngle;
	double aerodynamicTorque;
	double cplambda3;
	double tipSpeedRatio;
	ikSurf *surfCplambda3; /**<surface giving Cp/lambda^3 as a function of lambda and beta*/
	ikNotchList rotorSpeedNotchFilters;
	ikNotchList generatorTorqueNotchFilters;
	ikNotchList pitchAngleNotchFilters;
	ikTfList rotorSpeedLowPassFilter;
	ikTfList generatorTorqueLowPassFilter;
	ikTfList pitchAngleLowPassFilter;
	ikTfList rotorSpeedDerivation;
        /* @endcond */
    } ikTsrEst;
    
    /**
     * @struct ikTsrEstParams
     * @brief Tip-speed ratio estimator initialisation parameters
     */
    typedef struct ikTsrEstParams {
	double b; /**<gearbox ratio, i.e. high-speed shaft speed divided by low-speed shaft speed*/
	double J; /**<rotor moment of inertia in kgm^2*/
	double rho; /**<air density in kg/m^3*/
	double R; /**<rotor radius in m*/
	double T; /**<time step in seconds*/
	ikNotchListParams notches; /**<notch filter initialisation parameters*/
	ikTfListParams lowPass; /**<low pass filter initialisation parameters*/
	const char *cplambda3SurfaceFileName; /**<name of a valid file for @link ikSurf_newf @endlink*/
    } ikTsrEstParams;
    
    /**
     * Initialise an instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li -1: rotor speed notch filters initialisation error
     * @li -2: generator torque notch filters initialisation error
     * @li -3: pitch angle notch filters initialisation error
     * @li -4: rotor speed low pass filter initialisation error
     * @li -5: generator torque low filter initialisation error
     * @li -6: pitch angle low filter initialisation error
     * @li -7: rotor speed derivation transfer function initialisation error
     * @li -8: Cplambda3 surface initialisation error
     */
    int ikTsrEst_init(ikTsrEst *self, const ikTsrEstParams *params);
    
    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikTsrEst_initParams(ikTsrEstParams *params);
    
    /**
     * Execute periodic calculations
     * @param self tip-speed ratio estimator instance
     * @param generatorSpeed generator speed in rad/s
     * @param generatorTorque generator torque in kNm
     * @param pitchAngle pitch angle in degrees
     * @return estimated tip-speed ratio
     */
    double ikTsrEst_step(ikTsrEst *self, double generatorSpeed, double generatorTorque, double pitchAngle);
    
    /**
     * Get output value by name. All signals named on the block diagram of
     * @link ikTsrEst @endlink are accessible. To refer to sub-block signals,
     * use the sub-block name followed, when applicable by a ">" character and
     * the signal name or a string representation of the applicable index.
     * For example:
     * @li to get the output of the rotor speed low pass filter, use "rotor speed low pass filter>0"
     * @li to get the rotor speed, use "rotor speed"
     *
     * @param self tip-speed ratio estimator instance
     * @param output output value
     * @param name output name
     * @return error code:
     * @li 0: no error
     * @li -1: invalid signal name
     * @li -2: invalid block name
     */
    int ikTsrEst_getOutput(const ikTsrEst *self, double *output, const char *name);

    /**
     * Delete instance
     * @param self tip-speed ratio estimator instance
     */
    void ikTsrEst_delete(ikTsrEst *self);


#ifdef __cplusplus
}
#endif

#endif /* IKTSREST_H */

