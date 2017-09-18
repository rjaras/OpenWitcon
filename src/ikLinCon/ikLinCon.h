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
 * @file ikLinCon.h
 * 
 * @brief Class ikLinCon interface
 */

#ifndef IKLINCON_H
#define IKLINCON_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ikTfList.h"
#include "ikNotchList.h"
#include "ikLutbl.h"
#include "ikSlti.h"
    
#define IKLINCON_MAXNCONFIG 8

    /**
     * @struct ikLinCon 
     * @brief Linear controller
     * 
     * Instances of this class take a demand and a measurement, to which they
     * apply a series of notch filters and transfer functions. The results
     * are then substracted, demand - measurement, to obtain an error. A gain
     * schedule and another series of notch filters and transfer functions are
     * then applied to this error, to obtain a control action. 
     * 
     * @par Inputs
     * @li demand, specify via @link ikLinCon_step @endlink
     * @li measurement, specify via @link ikLinCon_step @endlink
     * @li gain schedule key, specify via `gainSchedXVal` at @link ikLinConParams @endlink
     * @li maximum post-gain value, specify via `maxPostGainValue` at @link ikLinConParams @endlink
     * @li minimum post-gain value, specify via `minPostGainValue` at @link ikLinConParams @endlink
     * @li maximum control action, specify via `maxControlAction` at @link ikLinConParams @endlink
     * @li minimum control action, specify via `minControlAction` at @link ikLinConParams @endlink
     * 
     * @par Outputs
     * @li control action, get via @link ikLinCon_step @endlink
     * 
     * @par Unit block
     * 
     * @image html ikLinCon_unit_block.svg
     * 
     * @par Block diagram
     * 
     * @image html ikLinCon_block_diagram.svg
     * 
     * @par Methods
     * @li @link ikLinCon_init @endlink initialise an instance
     * @li @link ikLinCon_step @endlink execute periodic calculations
     * @li @link ikLinCon_getOutput @endlink get output value
     * 
     * @cond
     * The flow is as follows:
     *                             
     * 
     *        demand                  measurement
     *          |                          |
     *          |                          |
     *          ▼                         ▼
     *    [notch filters]            [notch filters]
     *          |                          |
     *          |                          |
     *          ▼                         ▼
     * [transfer functions]       [transfer functions]
     *          |                          |
     *          |                          |
     *          └--------->(-)<-----------┘
     *                       |
     *                       | error
     *                       ▼
     *           [error transfer functions]
     *                       |
     *                       |
     *                       ▼
     *                 [gain schedule]
     *                       |
     *                       |
     *                       ▼
     *          [post-gain transfer functions]
     *                       |
     *                       |
     *                       ▼
     *                 control action
     * 
     * @endcond
     *    
     */                                                                          
    typedef struct ikLinCon {
        /**
         * Private members
         */
        /* @cond */
        ikTfList    postGainTfList;
        ikTfList    errorTfList;
        ikTfList    measurementTfList;
        ikTfList    demandTfList;
        ikNotchList measurementNotchList;
        ikNotchList demandNotchList;
        ikLutbl     gainSched;
        double      *gainSchedX;
        double      demand;
        double      filteredDemand;
        double      measurement;
        double      filteredMeasurement;
        double      gainSchedOutput;
        int         demandTfsEnable                 [IKLINCON_MAXNCONFIG] [IKTFLIST_NMAX];
        int         measurementTfsEnable            [IKLINCON_MAXNCONFIG] [IKTFLIST_NMAX];
        int         errorTfsEnable                  [IKLINCON_MAXNCONFIG] [IKTFLIST_NMAX];
        int         demandNotchesEnable             [IKLINCON_MAXNCONFIG] [IKNOTCHLIST_NMAX];
        int         measurementNotchesEnable        [IKLINCON_MAXNCONFIG] [IKNOTCHLIST_NMAX];
        int         configN;
        int         *config;
        int         currentDemandTfsEnable          [IKTFLIST_NMAX];
        int         currentMeasurementTfsEnable     [IKTFLIST_NMAX];
        int         currentErrorTfsEnable           [IKTFLIST_NMAX];
        int         currentDemandNotchesEnable      [IKNOTCHLIST_NMAX];
        int         currentMeasurementNotchesEnable [IKNOTCHLIST_NMAX];
        /* @endcond */
    } ikLinCon;
    
    /**
     * @struct ikLinConParams
     * @brief Linear controller initialisation parameters
     */
    typedef struct ikLinConParams {
        ikTfListParams      demandTfs;                                     /**<demand transfer function initialisation parameters.
                                                                                     The default value corresponds to a static unit gain.*/
        ikTfListParams      measurementTfs;                                /**<measurement transfer function initialisation parameters.
                                                                                     The default value corresponds to a static unit gain.*/
        ikTfListParams      errorTfs;                                      /**<error transfer function initialisation parameters.
                                                                                     The default value corresponds to a static unit gain.*/
        ikTfListParams      postGainTfs;                                    /**<post-gain transfer function initialisation parameters.
                                                                                    The default value corresponds to a static unit gain.*/
        ikNotchListParams   demandNotches;                                 /**<demand notch filter initialisation parameters.
                                                                                     The default value corresponds to a static unit gain.*/
        ikNotchListParams   measurementNotches;                            /**<measurement notch filter initialisation parameters.
                                                                                    The default value corresponds to a static unit gain.*/
        double              *maxControlAction;                             /**<pointer to a persistent address where the maximum control action
                                                                                    value is maintained. If NULL, no upper control action limit is applied.
                                                                                    The default value is NULL.*/
        double              *minControlAction;                             /**<pointer to a persistent address where the minimum control action
                                                                                    value is maintained. If NULL, no lower control action limit is applied.
                                                                                    The default value is NULL.*/
        double              *maxPostGainValue;                             /**<pointer to a persistent address where the maximum post-gain
                                                                                    value is maintained. If NULL, no upper post-gain value limit is applied.
                                                                                    The default value is NULL.*/
        double              *minPostGainValue;                             /**<pointer to a persistent address where the minimum post-gain
                                                                                    value is maintained. If NULL, no lower post-gain value limit is applied.
                                                                                    The default value is NULL.*/
        int                 gainSchedN;                                    /**<number of points defining the gain shedule.
                                                                                     The default value is 2.*/
        double              gainSchedX              [IKLUTBL_MAXPOINTS];    /**<keys of the points defining the gain schedule.
                                                                                     The default value is {-1.0, 1.0, 0.0, 0.0, ...}*/
        double              gainSchedY              [IKLUTBL_MAXPOINTS];    /**<values of the points defining the gain schedule.
                                                                                     The default value is {-1.0, 1.0, 0.0, 0.0, ...}*/
        double              *gainShedXVal;                                 /**<pointer to a persistent address where the input to
                                                                                     the gain schedule look-up table is maintained. If NULL,
                                                                                     no gain schedule will be applied. The default value is NULL.*/
        int                 demandTfsEnable         [IKLINCON_MAXNCONFIG] [IKTFLIST_NMAX];    /**<preset enable settings for demand transfer functions*/
        int                 measurementTfsEnable    [IKLINCON_MAXNCONFIG] [IKTFLIST_NMAX];    /**<preset enable settings for measurement transfer functions*/
        int                 errorTfsEnable          [IKLINCON_MAXNCONFIG] [IKTFLIST_NMAX];    /**<preset enable settings for error transfer functions*/
        int                 demandNotchesEnable     [IKLINCON_MAXNCONFIG] [IKNOTCHLIST_NMAX];    /**<preset enable settings for demand notch filters*/
        int                 measurementNotchesEnable[IKLINCON_MAXNCONFIG] [IKNOTCHLIST_NMAX];    /**<preset enable settings for measurement notch filters*/
        int                 configN;                                        /**<number of preset configurations. It must be between 0 and @link IKLINCON_MAXNCONFIG @endlink.
                                                                             Any value outside of said range will make the presets not to be used.*/
        int                 *config;                                        /**<pointer to a persistent memory address where the preset selection is maintained.
                                                                             A preset configuration will be selected according to the value stored in said address.
                                                                             Set to NULL to disable presets.*/
        
    } ikLinConParams;
    
    /**
     * Initialise instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li -1: could not initialise demand transfer functions
     * @li -2: could not initialise measurement transfer functions
     * @li -3: could not initialise error transfer functions
     * @li -4: could not initialise demand notch filters
     * @li -5: could not initialise measurement notch filters
     * @li -6: could not initialise post-gain transfer functions
     * @li -7: could not initialise gain schedule
     * @li -8: could not initialise enable presets
     */
    int ikLinCon_init(ikLinCon *self, const ikLinConParams *params);
    
    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikLinCon_initParams(ikLinConParams *params);
    
    /**
     * Execute periodic calculations
     * @param self linear controller instance
     * @param demand demand value
     * @param measurement measurement value
     * @return control action value
     */
    double ikLinCon_step(ikLinCon *self, double demand, double measurement);
    
    /**
     * Get output value by name. All signals named in the block diagram of
     * @link ikLinCon @endlink are accessible. To refer to sub-block signals,
     * use the sub-block name followed, when applicable by a ">" character and
     * the signal name or a string representation of the applicable index.
     * For example:
     * @li to get the output of the last but one transfer function to be applied
     * on the error signal, use "error transfer functions>1"
     * @li to get the output of the gain schedule, use "gain schedule"
     * @li to get the output of the last notch filter to be applied on the demand
     * signal, use "demand notch filters>0" or "demand notch filters"
     * 
     * @param self linear control instance
     * @param output output value
     * @param name output name
     * @return error code:
     * @li 0: no error
     * @li -1: invalid signal name
     * @li -2: invalid block name
     */
    int ikLinCon_getOutput(const ikLinCon *self, double *output, const char *name);


#ifdef __cplusplus
}
#endif

#endif /* IKLINCON_H */

