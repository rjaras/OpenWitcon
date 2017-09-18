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
 * @file ikRegionSelector.h
 * 
 * @brief Class ikRegionSelector interface
 */

#ifndef IKREGIONSELECTOR_H
#define IKREGIONSELECTOR_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define IKREGIONSELECTOR_MAXREG 8
#define IKREGIONSELECTOR_MAXPOINTS 16
    
    /**
     * @struct ikRegionSelectorPoint
     * @brief coordinates of a point defining a region
     */
    typedef struct ikRegionSelectorPoint {
        double x; /**<coordinate x*/
        double y; /**<coordinate y*/
    } ikRegionSelectorPoint;
    
    /**
     * @struct ikRegionSelectorRegion
     * @brief region initialisation parameters
     */
    typedef struct ikRegionSelectorRegion {
        int                     nPoints;    /**<number of points defining the polygonal region*/
        ikRegionSelectorPoint   points  [IKREGIONSELECTOR_MAXPOINTS];   /**<points defining the polygonal region*/
    } ikRegionSelectorRegion;

    /**
     * @struct ikRegionSelector
     * @brief selector of regions on 2-dimensional plane
     * 
     * Instances of this type store polygonal regions on a two-dimensional plane,
     * and select the one which corresponds to the coordinates passed via
     * @link ikRegionSelector_getRegion @endlink.
     * 
     * @par Inputs
     * @li x, specify via @link ikRegionSelector_getRegion @endlink
     * @li y, specify via @link ikRegionSelector_getRegion @endlink
     * 
     * @par outputs
     * @li region number, returned by @link ikRegionSelector_getRegion @endlink
     * 
     * @par Methods
     * @li @link ikRegionSelector_init @endlink initialise instance
     * @li @link ikRegionSelector_initParams @endlink initialise initialisation parameter structure
     * @li @link ikRegionSelector_getRegion @endlink get region number corresponding a pair of corrdinates
     */
    typedef struct ikRegionSelector {
        /* @cond */
        int                     regionN;
        ikRegionSelectorRegion  regions     [IKREGIONSELECTOR_MAXREG];
        /* @endcond */
    } ikRegionSelector;
    
    /**
     * @struct ikRegionSelectorParams
     * @brief region selector initialisation parameters
     */
    typedef struct ikRegionSelectorParams {
        int                     nRegions;   /**<number of regions.*/
        ikRegionSelectorRegion  regions     [IKREGIONSELECTOR_MAXREG]; /**<polygonal region specification*/
    }ikRegionSelectorParams;
    
    /**
     * Initialise instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li -1: invalid number of regions, it must be between 0 and @link IKREGIONSELECTOR_MAXREG @endlink
     * @li x: invalid number of points in region x (starting at 1), it must be between 0 and @link IKREGIONSELECTOR_MAXPOINTS @endlink
     */
    int ikRegionSelector_init(ikRegionSelector *self, const ikRegionSelectorParams *params);
    
    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikRegionSelector_initParams(ikRegionSelectorParams *params);
    
    /**
     * Get region number for coordinates
     * @param self region selector instance
     * @param x coordinate x
     * @param y coordinate y
     * @return region number, starting at 1. If more than one region contains the coordinates,
     * returns the first to feature in the initialisation parameters passed to
     * @link ikRegionSelector_init @endlink. If no region contains the coordinates,
     * returns 0.
     */
    int ikRegionSelector_getRegion(ikRegionSelector *self, double x, double y);


#ifdef __cplusplus
}
#endif

#endif /* IKREGIONSELECTOR_H */

