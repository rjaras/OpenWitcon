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
 * @file ikStpgen.c
 * 
 * @brief Class ikStpgen implementation
 */

/* @cond */

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "../ikStpgen/ikStpgen.h"

int ikStpgen_init(ikStpgen *self, const ikStpgenParams *params) {
    int i;

    /* declare error code */
    int err = 0;

    /* initialise state machine */
    self->state = 0;

    /* initialise inputs and outputs */
    self->r = 0.0;
    self->minCon = 0.0;
    self->maxCon = 0.0;
    self->feedback = 0.0;
    self->controlAction = 0.0;
    self->externalMaximumControlAction = 0.0;
    self->externalMinimumControlAction = 0.0;
    self->externalMaximumSetpoint = 0.0;
    self->uopt = 0.0;

    /* register open loop gain sign */
    if (0 > params->openLoopGainSign) {
        self->olsign = -1;
    } else {
        self->olsign = 1;
    }
    
    /* register preferred control action*/
    self->pUopt = params->preferredControlAction;

    /* register zones */
    self->nzones = params->nzones;
    if (params->nzones < 0) {
        self->nzones = 0;
        if (!err) err = -4;
    }
    if (params->nzones > IKSTPGEN_NZONEMAX) {
        self->nzones = IKSTPGEN_NZONEMAX;
        if (!err) err = -4;
    }
    for (i = 0; i < params->nzones; i++) {
        if (params->setpoints[0][i] >= params->setpoints[1][i]) {
            self->nzones = 0;
            if (!err) err = -5;
        }
    }
    for (i = 1; i < params->nzones; i++) {
        if (params->setpoints[0][i] <= params->setpoints[1][i - 1]) {
            self->nzones = 0;
            if (!err) err = -5;
        }
    }
    for (i = 0; i < params->nzones; i++) {
        self->setpoints[0][i] = params->setpoints[0][i];
        self->setpoints[1][i] = params->setpoints[1][i];
    }
    
    /* register zone transition hysteresis values*/
    for (i = 0; i < params->nzones - 1; i++) {
        if (params->zoneTransitionHysteresis[i] < 0.0) {
            self->nzones = 0;
            if (!err) err = -3;
        }
    }
    for (i = 0; i < params->nzones - 1; i++) {
        self->zoneTransitionHysteresis[i] = params->zoneTransitionHysteresis[i];
    }

    /* register zone transition step numbers */
    for (i = 0; i < params->nzones - 1; i++) {
        if (params->nZoneTransitionSteps[i] < 0) {
            self->nzones = 0;
            if (!err) err = -6;
        }
    }
    for (i = 0; i < params->nzones - 1; i++) {
        self->nZoneTransitionSteps[i] = params->nZoneTransitionSteps[i];
        self->iZoneTransitionSteps[i] = 0;
    }

    /* register zone transition lock step numbers */
    for (i = 0; i < params->nzones - 1; i++) {
        if (params->nZoneTransitionLockSteps[i] < 0) {
            self->nzones = 0;
            if (!err) err = -7;
        }
    }
    for (i = 0; i < params->nzones - 1; i++) {
        self->nZoneTransitionLockSteps[i] = params->nZoneTransitionLockSteps[i];
        self->iZoneTransitionLockSteps[i] = 0;
    }
    
    /* pre-lock transitions */
    if (0 != params->zoneTransitionPrelock && 1 != params->zoneTransitionPrelock) {
        self->nzones = 0;
        if (!err) err = -9;
    }
    for (i = 0; i < params->nzones - 1; i++) {
        self->zoneTransitionLocked[i] = params->zoneTransitionPrelock;
    }
    
    /* register control action limit rate */
    if (0.0 > params->controlActionLimitRate) {
        self->controlActionLimitRate = 0.0;
        if(!err) err = -8;
    } else {
        self->controlActionLimitRate = params->controlActionLimitRate;
    }    

    return err;
}

void ikStpgen_initParams(ikStpgenParams *params) {
    int i;

    /* set open loop gain to negative */
    params->openLoopGainSign = -1;
    
    /* disable preferred control action*/
    params->preferredControlAction = NULL;

    /* disable setpoint hopping */
    params->nzones = 0;
    for (i = 0; i < IKSTPGEN_NZONEMAX; i++) {
        params->setpoints[0][i] = 0.0;
        params->setpoints[1][i] = 0.0;
    }
    
    /* disable zone transition hysteresis */
    for (i = 0; i < IKSTPGEN_NZONEMAX - 1; i++) {
        params->zoneTransitionHysteresis[i] = 0.0;
    }

    /* make setpoint hopping immediate */
    for (i = 0; i < IKSTPGEN_NZONEMAX - 1; i++) {
        params->nZoneTransitionSteps[i] = 0;
    }
    
    /* unlock transitions */
    for (i = 0; i < IKSTPGEN_NZONEMAX - 1; i++) {
        params->nZoneTransitionLockSteps[i] = 0;
    }
    
    /* pre-lock transitions */
   params->zoneTransitionPrelock = 1;
    
    /* make control action limit changes immediate */
    params->controlActionLimitRate = 0.0;
}

double ikStpgen_step(ikStpgen *self, double maxSp, double feedback, double conAct, double minCon, double maxCon) {
    int zone;

    /* register inputs */
    self->feedback = feedback;
    self->controlAction = conAct;
    self->externalMaximumControlAction = maxCon;
    self->externalMinimumControlAction = minCon;
    self->externalMaximumSetpoint = maxSp;
    
    /* register the preferred control action*/
    if (NULL != self->pUopt) {
        self->uopt = *(self->pUopt);
    } else {
        self->uopt = 0.0;
    }

    /* if there are no zones, wave the maximum setpoint through */
    if (1 > self->nzones) {
        self->r = maxSp;
        self->maxCon = maxCon;
        self->minCon = minCon;
        return self->r;
    }

    /* update state */
    zone = (int) floor(self->state / 4 + 0.1);
    switch (self->state - 4 * zone) {
        case 0:
            if ((zone > 0) && (maxSp < self->setpoints[0][zone])) {
                self->state -= 2;
                break;
            }

            if (((self->setpoints[0][zone] + self->setpoints[1][zone]) / 2 < self->feedback) && (maxSp > self->setpoints[1][zone])) {
                self->state += 2;
                break;
            }

            if (((self->setpoints[0][zone] + maxSp) / 2 < self->feedback) && (maxSp > self->setpoints[0][zone])) {
                self->state += 1;
                break;
            }

            if ((zone > 0)
                    &&
                    ((self->controlAction - self->uopt)*self->olsign > self->zoneTransitionHysteresis[zone-1])
                    &&
                    (self->iZoneTransitionSteps[zone - 1] >= self->nZoneTransitionSteps[zone - 1])
                    &&
                    (0 >= self->iZoneTransitionLockSteps[zone - 1])) {
                self->state -= 2;
                self->iZoneTransitionLockSteps[zone - 1] = self->nZoneTransitionLockSteps[zone - 1];
                break;
            }
            break;

        case 1:
            if (maxSp > self->setpoints[1][zone]) {
                self->state += 1;
                break;
            }

            if (((self->setpoints[0][zone] + maxSp) / 2 > self->feedback) || (maxSp < self->setpoints[0][zone])) {
                self->state -= 1;
                break;
            }
            break;

        case 2:
            if (maxSp < self->setpoints[1][zone]) {
                self->state -= 1;
                break;
            }

            if ((self->setpoints[0][zone] + self->setpoints[1][zone]) / 2 > self->feedback) {
                self->state -= 2;
                break;
            }

            if ((zone < self->nzones - 1)
                    &&
                    ((self->uopt - self->controlAction)*self->olsign > self->zoneTransitionHysteresis[zone])
                    &&
                    (0 >= self->iZoneTransitionSteps[zone])
                    &&
                    (!self->zoneTransitionLocked[zone] || (0 >= self->iZoneTransitionLockSteps[zone]))
                    &&
                    (maxSp > self->setpoints[0][zone+1])) {
                self->state += 2;
                self->iZoneTransitionLockSteps[zone] = self->nZoneTransitionLockSteps[zone];
                self->zoneTransitionLocked[zone] = 1;
                break;
            }
            break;
    }

    /* do what has to be done depending on the state */
    zone = (int) floor(self->state / 4 + 0.1);
    switch (self->state - 4 * zone) {
        case 0:
            if (0 < zone) {
                if ((self->controlAction - self->uopt)*self->olsign > self->zoneTransitionHysteresis[zone-1]) {
                    if ((self->iZoneTransitionSteps[zone - 1] >= self->nZoneTransitionSteps[zone - 1])
                            &&
                            (0 < self->iZoneTransitionLockSteps[zone - 1]))
                        self->iZoneTransitionLockSteps[zone - 1]--;
                } else {
                    self->iZoneTransitionLockSteps[zone - 1] = self->nZoneTransitionLockSteps[zone - 1];
                }
                if (self->iZoneTransitionSteps[zone - 1] < self->nZoneTransitionSteps[zone - 1]) self->iZoneTransitionSteps[zone - 1]++;
            }
            if (0 < zone && self->nZoneTransitionSteps[zone - 1] > 0) {
                self->r = self->setpoints[1][zone - 1] + ((double)self->iZoneTransitionSteps[zone - 1]) / self->nZoneTransitionSteps[zone - 1] * (self->setpoints[0][zone] - self->setpoints[1][zone - 1]);
            } else {
                self->r = self->setpoints[0][zone];
            }
            if (0 > self->olsign) {
                self->maxCon = self->uopt;
                if (0.0 < self->controlActionLimitRate) {
                    self->maxCon = self->maxCon > conAct - self->controlActionLimitRate ? self->maxCon : conAct - self->controlActionLimitRate;
                    self->maxCon = self->maxCon < conAct + self->controlActionLimitRate ? self->maxCon : conAct + self->controlActionLimitRate;
                }
                self->minCon = minCon;
                self->maxCon = self->maxCon > self->minCon ? self->maxCon : self->minCon;
            } else {
                self->maxCon = maxCon;
                self->minCon = self->uopt;
                if (0.0 < self->controlActionLimitRate) {
                    self->minCon = self->minCon < conAct + self->controlActionLimitRate ? self->minCon : conAct + self->controlActionLimitRate;
                    self->minCon = self->minCon > conAct - self->controlActionLimitRate ? self->minCon : conAct - self->controlActionLimitRate;
                }
                self->minCon = self->minCon < self->maxCon ? self->minCon : self->maxCon;
            }
            if (0 >= zone && maxSp < self->setpoints[0][0]) {
                self->r = maxSp;
                self->maxCon = maxCon;
                self->minCon = minCon;
            }
            break;
        case 1:
            self->r = maxSp;
            if (0 > self->olsign) {
                self->maxCon = maxCon;
                self->minCon = self->uopt;
                if (0.0 < self->controlActionLimitRate) {
                    self->minCon = self->minCon < conAct + self->controlActionLimitRate ? self->minCon : conAct + self->controlActionLimitRate;
                    self->minCon = self->minCon > conAct - self->controlActionLimitRate ? self->minCon : conAct - self->controlActionLimitRate;
                }
                self->minCon = self->minCon < self->maxCon ? self->minCon : self->maxCon;
            } else {
                self->maxCon = self->uopt;
                if (0.0 < self->controlActionLimitRate) {
                    self->maxCon = self->maxCon > conAct - self->controlActionLimitRate ? self->maxCon : conAct - self->controlActionLimitRate;
                    self->maxCon = self->maxCon < conAct + self->controlActionLimitRate ? self->maxCon : conAct + self->controlActionLimitRate;
                }
                self->minCon = minCon;
                self->maxCon = self->maxCon > self->minCon ? self->maxCon : self->minCon;
            }
            break;
        case 2:
            if (zone < self->nzones - 1) {
                if ((self->uopt - self->controlAction)*self->olsign > self->zoneTransitionHysteresis[zone]) {
                    if (0 >= self->iZoneTransitionSteps[zone]
                            &&
                            0 < self->iZoneTransitionLockSteps[zone])
                        self->iZoneTransitionLockSteps[zone]--;
                } else {
                    self->iZoneTransitionLockSteps[zone] = self->nZoneTransitionLockSteps[zone];
                }
                if (0 < self->iZoneTransitionSteps[zone]) self->iZoneTransitionSteps[zone]--;
            }
            if (self->nzones - 1 > zone && self->nZoneTransitionSteps[zone] > 0) {
                self->r = self->setpoints[1][zone] + ((double)self->iZoneTransitionSteps[zone]) / self->nZoneTransitionSteps[zone] * (self->setpoints[0][zone + 1] - self->setpoints[1][zone]);
            } else {
                self->r = self->setpoints[1][zone];
            }
            if (0 > self->olsign) {
                self->maxCon = maxCon;
                self->minCon = self->uopt;
                if (0.0 < self->controlActionLimitRate) {
                    self->minCon = self->minCon < conAct + self->controlActionLimitRate ? self->minCon : conAct + self->controlActionLimitRate;
                    self->minCon = self->minCon > conAct - self->controlActionLimitRate ? self->minCon : conAct - self->controlActionLimitRate;
                }
            } else {
                self->maxCon = self->uopt;
                if (0.0 < self->controlActionLimitRate) {
                    self->maxCon = self->maxCon > conAct - self->controlActionLimitRate ? self->maxCon : conAct - self->controlActionLimitRate;
                    self->maxCon = self->maxCon < conAct + self->controlActionLimitRate ? self->maxCon : conAct + self->controlActionLimitRate;
                }
                self->minCon = minCon;
            }
            break;
    }

    self->maxCon = self->maxCon < maxCon ? self->maxCon : maxCon;
    self->minCon = self->minCon > minCon ? self->minCon : minCon;

    /* return setpoint */
    return self->r;
}

int ikStpgen_getOutput(const ikStpgen *self, double *output, const char *name) {
    const char *sep;

    /* pick up the signals */
    if (!strcmp(name, "feedback")) {
        *output = self->feedback;
        return 0;
    }
    if (!strcmp(name, "control action")) {
        *output = self->controlAction;
        return 0;
    }
    if (!strcmp(name, "external maximum control action")) {
        *output = self->externalMaximumControlAction;
        return 0;
    }
    if (!strcmp(name, "external minimum control action")) {
        *output = self->externalMinimumControlAction;
        return 0;
    }
    if (!strcmp(name, "external maximum setpoint")) {
        *output = self->externalMaximumSetpoint;
        return 0;
    }
    if (!strcmp(name, "setpoint")) {
        *output = self->r;
        return 0;
    }
    if (!strcmp(name, "maximum control action")) {
        *output = self->maxCon;
        return 0;
    }
    if (!strcmp(name, "minimum control action")) {
        *output = self->minCon;
        return 0;
    }
    if (!strcmp(name, "preferred control action")) {
        *output = self->uopt;
        return 0;
    }

    return -1;
}


/* @endcond */
