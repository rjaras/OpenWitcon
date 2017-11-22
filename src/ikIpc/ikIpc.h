/*
Copyright (C) 2017 IK4-IKERLAN

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
 * @file ikIpc.h
 * 
 * @brief Class ikIpc interface
 */

#ifndef IKIPC_H
#define IKIPC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ikConLoop.h"
#include "ikVector.h"
    
    /**
     * @struct ikIpcInputs
     * @brief Individual pitch control inputs
     * 
     * Instances of ikIpc take inputs via a structure of this type: @link ikIpc.in @endlink.
     */
    typedef struct ikIpcInputs {
        double azimuth; /**<azimuth angle (\f$\theta\f$), in degrees. Positive values correspond to
                         counter-clockwise roation of the rotor as seen from downwind.*/
        double collectivePitch; /**<collective pitch angle, in degrees.*/
        double maximumPitch; /**<maximum pitch, in degrees.*/
        double minimumPitch; /**<minimum pitch, in degrees.*/
        ikVector bladeRootMoments[3]; /**<blade root moments, in kNm, expressed in coordinates
                                       * of the corresponding rotating frames of reference
                                       * as defined in @link ikIpc @endlink. This is an array
                                       * of 3 instances of @link ikVector @endlink, each
                                       * with 3 coordinates. The coordinates in the first
                                       * @link ikVector @endlink correspond to blade 1
                                       * root moments around local axes \f$ x' \f$, 
                                       * \f$ y' \f$ and \f$ z' \f$, in that order.
                                       * The second and third instance of @link ikVector @endlink
                                       * have the homologous information regarding
                                       * blades 2 and 3, respectively. */
        double demandedMy; /**<demanded My, in kNm*/
        double demandedMz; /**<demanded Mz, in kNm*/
        double maximumIndividualPitch; /**<maximum individual pitch, in degrees*/
        double externalPitchY; /**<external pitch y, in degrees*/
        double externalPitchZ; /**<external pitch z, in degrees*/
    } ikIpcInputs;
    
    /**
     * @struct ikIpcOutputs
     * @brief Individual pitch control outputs
     * 
     * Instances of ikIpc make outputs available via a structure of this type: @link ikIpc.out @endlink.
     */
    typedef struct ikIpcOutputs {
        double pitch[3]; /**<pitch angles, in degrees*/
    } ikIpcOutputs;
    
    /* @cond */
    typedef struct ikIpcPrivate {
        double pitchDifferentials[3];
        double azimuthOffsets[3];
        ikConLoop conMz;
        ikConLoop conMy;
        ikVector staticMoment;
        double pitchYcon;
        double pitchZcon;
        ikVector staticPitch;
        double aggregatePitch[3];
    } ikIpcPrivate;
    /* @endcond */
    
    /**
     * @struct ikIpc
     * @brief Individual pitch control
     * 
     * Instances of this type are wind turbine individual pitch controllers.
     * 
     * @par Inputs
     * @li azimuth, specify via @link in @endlink
     * @li collective pitch, specify via @link in @endlink
     * @li maximum pitch, specify via @link in @endlink
     * @li minimum pitch, specify via @link in @endlink
     * @li blade root moments, specify via @link in @endlink
     * @li demanded My, specify via @link in @endlink
     * @li demanded Mz, specify via @link in @endlink
     * @li maximum individual pitch, specify via @link in @endlink
     * @li external pitch y, specify via @link in @endlink
     * @li external pitch z, specify via @link in @endlink
     * 
     * @par Outputs
     * @li pitch, get via @link out @endlink
     * 
     * @par Unit block
     * 
     * @image html ikIpc_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikIpc_block_diagram.svg
     * 
     * @par Frames of reference
     * 
     * The non-rotating frame of reference is defined by axes \f$ x \f$, \f$ y \f$
     * and \f$ z \f$. \f$ x \f$ points downwind, \f$ z \f$ points up and \f$ y \f$
     * complies with the right hand rule.
     * 
     * The rotating frame of reference local to blade 1 is defined by axes \f$ x' \f$, \f$ y' \f$
     * and \f$ z' \f$. \f$ x' \f$ points downwind, \f$ z' \f$ points from the blade
     * root to the blade tip and \f$ y' \f$ complies with the right hand rule.
     * 
     * Therefore, as shown below, axes \f$ x \f$ and \f$ x' \f$ coincide, and the
     * rotating frame of reference rotates around them.
     * 
     * @image html ikIpc_coordinates_bis.svg
     * 
     * The rotating frames of reference local to blades 2 and 3 are defined by axes
     * \f$ \left\{ x'', y'', z'' \right\} \f$ and \f$ \left\{ x''', y''', z''' \right\} \f$,
     * respectively. \f$ x'' \f$ and \f$ x''' \f$ also coincide with \f$ x \f$,
     * \f$ z'' \f$ and \f$ z''' \f$ also point from their respective blade roots
     * to their respective blade tips, and \f$ y'' \f$ and \f$ y''' \f$ also comply
     * with the right hand rule.
     * 
     * The three rotating frames of reference local to blades 1, 2 and 3, respectively,
     * rotate in unison, and are permanently 120º from each other, as shown below.
     * Note that two arrangements are possible.
     * 
     * To accommodate different turbine-specific idiosyncrasies, the azimuth angle
     * \f$ \theta \f$ is defined as the positive rotation around \f$ x \f$ necessary
     * to bring \f$ z \f$ to coincide with \f$ z' \f$, minus constant angle
     * \f$ \phi \f$, which may be specified via @link ikIpc_init @endlink. Similarly,
     * the position of blades 2 and 3 relative to that of blade 1 is defined by
     * parameter \f$ s \f$, as shown below. \f$ s \f$ may also be specified via
     * @link ikIpc_init @endlink.
     * 
     * @image html ikIpc_coordinates.svg
     * 
     * @par Methods
     * @li @link ikIpc_initParams @endlink initialise initialisation parameter structure
     * @li @link ikIpc_init @endlink initialise an instance
     * @li @link ikIpc_step @endlink execute preriodic calculations
     * @li @link ikIpc_getOutput @endlink get output value
     */
    typedef struct ikIpc {
        ikIpcInputs in; /**<inputs*/
        ikIpcOutputs out; /**<outputs*/
        /* @cond */
        ikIpcPrivate priv;
        /* @endcond */
    } ikIpc;
    
    /**
     * @struct ikIpcParams
     * @brief individual pitch control initialisation parameters
     */
    typedef struct ikIpcParams {
        int bladeOrder; /**<parameter \f$s\f$ as defined in @link ikIpc @endlink. The default value is 1.*/
        double azimuthOffset; /**<parameter \f$\phi\f$ as defined in @link ikIpc @endlink. The default value is 0.0.*/
        ikConLoopParams controlMz; /**< Mz control initialisation parameters */
        ikConLoopParams controlMy; /**< My control initialisation parameters */
    } ikIpcParams;
    
    /**
     * Initialise an instance
     * @param self instance
     * @param params initialisation parameters.
     * @return error code:
     * @li 0: no error
     * @li -1: could not initialise My control
     * @li -2: could not initialise Mz control
     */
    int ikIpc_init(ikIpc *self, const ikIpcParams *params);

    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikIpc_initParams(ikIpcParams *params);
    
    /**
     * Execute periodic calculations
     * @param self individual pitch control instance
     */
    void ikIpc_step(ikIpc *self);
    
    /**
     * Get output value by name. All signals named on the block diagram of
     * @link ikIpc @endlink are accessible, except for inputs and outputs available
     * at @link ikIpc.in @endlink or @link ikIpc.out @endlink, respectively. To refer to sub-block
     * signals, use the sub-block name followed by a ">" character and the
     * signal name.
     * @param self individual pitch control instance
     * @param output output value
     * @param name output name
     * @return error code:
     * @li 0: no error
     * @li -1: invalid signal name
     * @li -2: invalid block name
     */
    int ikIpc_getOutput(const ikIpc *self, double *output, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* IKIPC_H */

