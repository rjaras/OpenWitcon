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

#include "ikRegionSelector.h"

/**
 * @file ikRegionSelector.c
 * 
 * @brief Class ikRegionSelector implementation
 */

/* @cond */

int ikRegionSelector_init(ikRegionSelector *self, const ikRegionSelectorParams *params) {
    int i;
    
    /* declare error code */
    int err = 0;
    
    /* register parameter values */
    self->regionN = params->nRegions;
    if ((0 > self->regionN) || (IKREGIONSELECTOR_MAXREG < self->regionN)) {
        self->regionN = 0;
        err = -1;
    }
    for (i = 0; i < self->regionN; i++) {
        self->regions[i] = params->regions[i];
        if ((0 > self->regions[i].nPoints) ||(IKREGIONSELECTOR_MAXPOINTS < self->regions[i].nPoints)) {
            self->regions[i].nPoints = 0;
            if (!err) err = 2;
        }
    }
    
    /* return error code */
    return err;
}

void ikRegionSelector_initParams(ikRegionSelectorParams *params) {
    int i;
    int j;
    
    /* no regions, and all zeros */
    params->nRegions = 0;
    for (i = 0; i < IKREGIONSELECTOR_MAXREG; i++) {
        params->regions[i].nPoints = 0;
        for (j = 0; j < IKREGIONSELECTOR_MAXPOINTS; j++) {
            params->regions[i].points[j].x = 0.0;
            params->regions[i].points[j].y = 0.0;
        }
    }
}

int ikRegionSelector_isPointInRegion(const ikRegionSelectorRegion *region, const ikRegionSelectorPoint *point) {
    /* This code is a modification of Dan Sunday's wn_PnPoly() */
    /* The following is a verbatim copy of the copyright notice on the original code: */
    /* */
    /* Copyright 2000 softSurfer, 2012 Dan Sunday */
    /* This code may be freely used, distributed and modified for any purpose */
    /* providing that this copyright notice is included with it. */
    /* SoftSurfer makes no warranty for this code, and cannot be held */
    /* liable for any real or imagined damage resulting from its use. */
    /* Users of this code must verify correctness for their application. */

    /* declare and initialise the winding number */
    int wn = 0;

    /* loop over all edges of the polygon */
    int i;
    int i1;
    for (i = 0; i < region->nPoints; i++) {
        i1 = i + 1;
        if (i1 > region->nPoints - 1) i1 = 0;
        if (region->points[i].y <= point->y) {
            if (region->points[i1].y > point->y) /* an upward crossing */
                if ((region->points[i1].x - region->points[i].x) * (point->y - region->points[i].y)
                        - (region->points[i1].y - region->points[i].y) * (point->x - region->points[i].x)
                        > 0) /* point left of edge */
                    wn++; /* count up */
                else
                    wn--; /* count down */
        } else {
            if (region->points[i1].y <= point->y) /* a downward crossing */
                if ((region->points[i1].x - region->points[i].x) * (point->y - region->points[i].y)
                        - (region->points[i1].y - region->points[i].y) * (point->x - region->points[i].x)
                        < 0) /* point right of edge */
                    wn--; /* count down */
                else
                    wn++; /* count up */
        }
    }

    return wn;
}

int ikRegionSelector_getRegion(ikRegionSelector *self, double x, double y) {
    /* check every region */
    int i;
    ikRegionSelectorPoint point;
    point.x = x;
    point.y = y;
    for (i = 0; i < self->regionN; i++) {
        if (ikRegionSelector_isPointInRegion(&(self->regions[i]), &point))
            return i + 1;
    }

    /* if the point is not in any of the regions, return 0 */
    return 0;
}

/* @endcond */
