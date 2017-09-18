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
    int err_;

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

    /* register open loop gain sign */
    if (0 > params->openLoopGainSign) {
        self->olsign = -1;
    } else {
        self->olsign = 1;
    }

    /* initialise filters */
    err_ = ikLinCon_init(&(self->yFilters), &(params->feedbackFilters));
    if (!err && err_) err = -1;
    err_ = ikLinCon_init(&(self->uFilters), &(params->controlActionFilters));
    if (!err && err_) err = -2;

    /* initialise preferred control action look-up table */
    ikLutbl_init(&(self->uopt));
    err_ = ikLutbl_setPoints(&(self->uopt), params->preferredControlActionLutblN, params->preferredControlActionLutblX, params->preferredControlActionLutblY);
    if (!err && err_) err = -3;

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

    /* make the filters static unit gains, with no mixing */
    ikLinCon_initParams(&(params->feedbackFilters));
    params->feedbackFilters.measurementTfs.tfParams[0].enable = 1;
    params->feedbackFilters.measurementTfs.tfParams[0].b[0] = 0.0;
    ikLinCon_initParams(&(params->controlActionFilters));
    params->controlActionFilters.measurementTfs.tfParams[0].enable = 1;
    params->controlActionFilters.measurementTfs.tfParams[0].b[0] = 0.0;

    /* set open loop gain to negative */
    params->openLoopGainSign = -1;

    /* make the preferred control action curve a unit gain */
    params->preferredControlActionLutblN = 2;
    params->preferredControlActionLutblX[0] = -1.0;
    params->preferredControlActionLutblX[1] = 1.0;
    params->preferredControlActionLutblY[0] = -1.0;
    params->preferredControlActionLutblY[1] = 1.0;

    /* disable setpoint hopping */
    params->nzones = 0;
    for (i = 0; i < IKSTPGEN_NZONEMAX; i++) {
        params->setpoints[0][i] = 0.0;
        params->setpoints[1][i] = 0.0;
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
    double filteredFeedback;
    double filteredControlAction;
    double uopt;
    double uoptSat;
    int zone;

    /* register inputs */
    self->feedback = feedback;
    self->controlAction = conAct;
    self->externalMaximumControlAction = maxCon;
    self->externalMinimumControlAction = minCon;
    self->externalMaximumSetpoint = maxSp;

    /* take step with the filters */
    filteredFeedback = ikLinCon_step(&(self->yFilters), feedback, conAct);
    filteredControlAction = ikLinCon_step(&(self->uFilters), conAct, feedback);

    /* if there are no zones, wave the maximum setpoint through */
    if (1 > self->nzones) {
        self->r = maxSp;
        self->maxCon = maxCon;
        self->minCon = minCon;
        return self->r;
    }

    /* update state */
    uopt = ikLutbl_eval(&(self->uopt), filteredFeedback);
    zone = (int) floor(self->state / 4 + 0.1);
    switch (self->state - 4 * zone) {
        case 0:
            if ((zone > 0) && (maxSp < self->setpoints[0][zone])) {
                self->state -= 2;
                break;
            }

            if (((self->setpoints[0][zone] + self->setpoints[1][zone]) / 2 < filteredFeedback) && (maxSp > self->setpoints[1][zone])) {
                self->state += 2;
                break;
            }

            if (((self->setpoints[0][zone] + maxSp) / 2 < filteredFeedback) && (maxSp > self->setpoints[0][zone])) {
                self->state += 1;
                break;
            }

            if ((zone > 0)
                    &&
                    ((ikLutbl_eval(&(self->uopt), self->setpoints[1][zone - 1]) - filteredControlAction) * self->olsign < 0)
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

            if (((self->setpoints[0][zone] + maxSp) / 2 > filteredFeedback) || (maxSp < self->setpoints[0][zone])) {
                self->state -= 1;
                break;
            }
            break;

        case 2:
            if (maxSp < self->setpoints[1][zone]) {
                self->state -= 1;
                break;
            }

            if ((self->setpoints[0][zone] + self->setpoints[1][zone]) / 2 > filteredFeedback) {
                self->state -= 2;
                break;
            }

            if ((zone < self->nzones - 1)
                    &&
                    ((ikLutbl_eval(&(self->uopt), self->setpoints[0][zone + 1]) - filteredControlAction) * self->olsign > 0)
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
                if ((ikLutbl_eval(&(self->uopt), self->setpoints[1][zone - 1]) - filteredControlAction) * self->olsign < 0) {
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
            uoptSat = ikLutbl_eval(&(self->uopt), self->setpoints[0][zone]);
            if (0 > self->olsign) {
                self->maxCon = uopt > uoptSat ? uopt : uoptSat;
                if (0.0 < self->controlActionLimitRate) {
                    self->maxCon = self->maxCon > conAct - self->controlActionLimitRate ? self->maxCon : conAct - self->controlActionLimitRate;
                    self->maxCon = self->maxCon < conAct + self->controlActionLimitRate ? self->maxCon : conAct + self->controlActionLimitRate;
                }
                self->minCon = minCon;
                self->maxCon = self->maxCon > self->minCon ? self->maxCon : self->minCon;
            } else {
                self->maxCon = maxCon;
                self->minCon = uopt < uoptSat ? uopt : uoptSat;
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
            uoptSat = ikLutbl_eval(&(self->uopt), maxSp);
            if (0 > self->olsign) {
                self->maxCon = maxCon;
                self->minCon = uopt < uoptSat ? uopt : uoptSat;
                if (0.0 < self->controlActionLimitRate) {
                    self->minCon = self->minCon < conAct + self->controlActionLimitRate ? self->minCon : conAct + self->controlActionLimitRate;
                    self->minCon = self->minCon > conAct - self->controlActionLimitRate ? self->minCon : conAct - self->controlActionLimitRate;
                }
                self->minCon = self->minCon < self->maxCon ? self->minCon : self->maxCon;
            } else {
                self->maxCon = uopt > uoptSat ? uopt : uoptSat;
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
                if ((ikLutbl_eval(&(self->uopt), self->setpoints[0][zone + 1]) - filteredControlAction) * self->olsign > 0) {
                    if (0 >= self->iZoneTransitionSteps[zone]
                            &&
                            0 < self->iZoneTransitionLockSteps[zone])
                        self->iZoneTransitionLockSteps[zone]--;
                    if (0 < self->iZoneTransitionSteps[zone]) self->iZoneTransitionSteps[zone]--;
                } else {
                    self->iZoneTransitionLockSteps[zone] = self->nZoneTransitionLockSteps[zone];
                }
            }
            if (self->nzones - 1 > zone && self->nZoneTransitionSteps[zone] > 0) {
                self->r = self->setpoints[1][zone] + ((double)self->iZoneTransitionSteps[zone]) / self->nZoneTransitionSteps[zone] * (self->setpoints[0][zone + 1] - self->setpoints[1][zone]);
            } else {
                self->r = self->setpoints[1][zone];
            }
            uoptSat = ikLutbl_eval(&(self->uopt), self->setpoints[1][zone]);
            if (0 > self->olsign) {
                self->maxCon = maxCon;
                self->minCon = uopt < uoptSat ? uopt : uoptSat;
                if (0.0 < self->controlActionLimitRate) {
                    self->minCon = self->minCon < conAct + self->controlActionLimitRate ? self->minCon : conAct + self->controlActionLimitRate;
                    self->minCon = self->minCon > conAct - self->controlActionLimitRate ? self->minCon : conAct - self->controlActionLimitRate;
                }
            } else {
                self->maxCon = uopt > uoptSat ? uopt : uoptSat;
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
    if (!strcmp(name, "filtered feedback")) {
        ikLinCon_getOutput(&(self->yFilters), output, "control action");
        return 0;
    }
    if (!strcmp(name, "filtered control action")) {
        ikLinCon_getOutput(&(self->uFilters), output, "control action");
        return 0;
    }
    if (!strcmp(name, "preferred control action")) {
        double fca;
        ikLinCon_getOutput(&(self->yFilters), &fca, "control action");
        *output = ikLutbl_eval(&(self->uopt), fca);
        return 0;
    }

    /* pick up the blocks */
    sep = strstr(name, ">");
    if (NULL == sep) return -1;
    if (!strncmp(name, "feedback filtering", strlen(name) - strlen(sep))) {
        if (ikLinCon_getOutput(&self->yFilters, output, sep + 1)) {
            return -1;
        } else {
            return 0;
        }
    }
    if (!strncmp(name, "control action filtering", strlen(name) - strlen(sep))) {
        if (ikLinCon_getOutput(&self->uFilters, output, sep + 1)) {
            return -1;
        } else {
            return 0;
        }
    }

    return -2;
}


/* @endcond */
