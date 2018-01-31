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
 * @file ikLinCon.c
 * 
 * @brief Class ikLinCon implementation
 */

/* @cond */

#include <stdlib.h>
#include <string.h>
#include "../ikLinCon/ikLinCon.h"

int ikLinCon_init(ikLinCon *self, const ikLinConParams *params) {
    ikTfListParams demandTfs;
    ikTfListParams measurementTfs;
    ikTfListParams errorTfs;
    ikTfListParams postGainTfs;
    ikNotchListParams demandNotches;
    ikNotchListParams measurementNotches;
    int i;
    int j;
        
    /* declare error code */
    int err = 0;
    int err_;
    
    /* initialise inputs */
    self->demand = 0.0;
    self->measurement = 0.0;
    self->gainSchedOutput = 0.0;

    /* register enable presets */
    if ((params->configN > IKLINCON_MAXNCONFIG)
            ||
            (params->configN < 0)) {
        self->configN = 0;
        self->config = NULL;
        err = -8;
    } else {
        self->configN = params->configN;
        self->config = params->config;
        for (j = 0; j < IKLINCON_MAXNCONFIG; j++) {
            for (i = 0; i < IKTFLIST_NMAX; i++) {
                self->demandTfsEnable[j][i] = params->demandTfsEnable[j][i];
                self->measurementTfsEnable[j][i] = params->measurementTfsEnable[j][i];
                self->errorTfsEnable[j][i] = params->errorTfsEnable[j][i];
            }
            for (i = 0; i < IKNOTCHLIST_NMAX; i++) {
                self->demandNotchesEnable[j][i] = params->demandNotchesEnable[j][i];
                self->measurementNotchesEnable[j][i] = params->measurementNotchesEnable[j][i];
            }
        }
    }

    /* make copies of the list initialisation parameters */
    demandTfs = params->demandTfs;
    measurementTfs = params->measurementTfs;
    errorTfs = params->errorTfs;
    demandNotches = params->demandNotches;
    measurementNotches = params->measurementNotches;
    postGainTfs = params->postGainTfs;
    
    /* pass saturation limit pointers to post-gain transfer functions */
    postGainTfs.tfParams[0].enable = 1;
    postGainTfs.tfParams[0].maxOutput = params->maxControlAction;
    postGainTfs.tfParams[0].minOutput = params->minControlAction;
    postGainTfs.tfParams[IKTFLIST_NMAX-1].enable = 1;
    postGainTfs.tfParams[IKTFLIST_NMAX-1].maxInput = params->maxPostGainValue;
    postGainTfs.tfParams[IKTFLIST_NMAX-1].minInput = params->minPostGainValue;

    /* if a preset selector handle has been specified, make all the lists check */
    /* the current enable settings */
    if (NULL != self->config) {
        int i;
        for (i = 0; i < IKTFLIST_NMAX; i++) {
            demandTfs.tfParams[i].variableEnable = &(self->currentDemandTfsEnable[i]);
            measurementTfs.tfParams[i].variableEnable = &(self->currentMeasurementTfsEnable[i]);
            errorTfs.tfParams[i].variableEnable = &(self->currentErrorTfsEnable[i]);
        }
        for (i = 0; i < IKNOTCHLIST_NMAX; i++) {
            demandNotches.notchParams[i].variableEnable = &(self->currentDemandNotchesEnable[i]);
            measurementNotches.notchParams[i].variableEnable = &(self->currentMeasurementNotchesEnable[i]);
        }
    }

    /* call ikTfList initialisation on all three lists */
    err_ = ikTfList_init(&(self->demandTfList), &demandTfs);
    if (!err && err_) err = -1;
    err_ = ikTfList_init(&(self->measurementTfList), &measurementTfs);
    if (!err && err_) err = -2;
    err_ = ikTfList_init(&(self->errorTfList), &errorTfs);
    if (!err && err_) err = -3;

    /* call ikNotchList initialisation on all three lists */
    err_ = ikNotchList_init(&(self->demandNotchList), &demandNotches);
    if (!err && err_) err = -4;
    err_ = ikNotchList_init(&(self->measurementNotchList), &measurementNotches);
    if (!err && err_) err = -5;
    err_ = ikTfList_init(&(self->postGainTfList), &postGainTfs);
    if (!err && err_) err = -6;

    /* initialise gain schedule */
    self->gainSchedX = params->gainShedXVal;
    ikLutbl_init(&(self->gainSched));
    err_ = ikLutbl_setPoints(&(self->gainSched), params->gainSchedN, params->gainSchedX, params->gainSchedY);
    if (!err && err_) err = -7;
    
    return err;
}

void ikLinCon_initParams(ikLinConParams *params) {
    int i;
    int j;
    
    /* call ikTfList parameter initialisation */
    ikTfList_initParams(&(params->postGainTfs));
    ikTfList_initParams(&(params->demandTfs));
    ikTfList_initParams(&(params->measurementTfs));
    ikTfList_initParams(&(params->errorTfs));

    /* call ikNotchList parameter initialisation */
    ikNotchList_initParams(&(params->demandNotches));
    ikNotchList_initParams(&(params->measurementNotches));

    /* set default values for gain schedule */
    params->gainShedXVal = NULL;
    params->gainSchedN = 2;
    params->gainSchedX[0] = -1.0;
    params->gainSchedX[1] = 1.0;
    params->gainSchedY[0] = -1.0;
    params->gainSchedY[1] = 1.0;
    for (i = 2; i < IKLUTBL_MAXPOINTS; i++) {
        params->gainSchedX[i] = 0.0;
        params->gainSchedY[i] = 0.0;
    }

    /* set default values for configurations */
    params->config = NULL;
    params->configN = 0;
    for (i = 0; i < IKLINCON_MAXNCONFIG; i++) {
        for (j = 0; j < IKTFLIST_NMAX; j++) {
            params->demandTfsEnable[i][j] = 0;
            params->measurementTfsEnable[i][j] = 0;
            params->errorTfsEnable[i][j] = 0;
        }
        for (j = 0; j < IKNOTCHLIST_NMAX; j++) {
            params->demandNotchesEnable[i][j] = 0;
            params->measurementNotchesEnable[i][j] = 0;
        }
    }
    
    /* set saturation limit pointers to null */
    params->maxControlAction = NULL;
    params->minControlAction = NULL;
    params->maxPostGainValue = NULL;
    params->minPostGainValue = NULL;

}

double ikLinCon_step(ikLinCon *self, double demand, double measurement) {
    int i;
    double demand_;
    double measurement_;
    double err;
        
    /* register inputs */
    self->demand = demand;
    self->measurement = measurement;

    /* use presets to set the current enable settings */
    if (NULL != self->config) {
        int config = *(self->config);
        if (0 > config) config = 0;
        if (self->configN - 1 < config) config = self->configN - 1;
        for (i = 0; i < IKTFLIST_NMAX; i++) {
            self->currentDemandTfsEnable[i] = self->demandTfsEnable[config][i];
            self->currentMeasurementTfsEnable[i] = self->measurementTfsEnable[config][i];
            self->currentErrorTfsEnable[i] = self->errorTfsEnable[config][i];
        }
        for (i = 0; i < IKNOTCHLIST_NMAX; i++) {
            self->currentDemandNotchesEnable[i] = self->demandNotchesEnable[config][i];
            self->currentMeasurementNotchesEnable[i] = self->measurementNotchesEnable[config][i];
        }
    }

    /* take step on demand path */
    demand_ = demand;
    demand_ = ikNotchList_step(&(self->demandNotchList), demand_);
    demand_ = ikTfList_step(&(self->demandTfList), demand_);
    self->filteredDemand = demand_;

    /* take step on measurement path */
    measurement_ = measurement;
    measurement_ = ikNotchList_step(&(self->measurementNotchList), measurement_);
    measurement_ = ikTfList_step(&(self->measurementTfList), measurement_);
    self->filteredMeasurement = measurement_;

    /* calculate error */
    err = demand_ - measurement_;

    /* take step on error path */
    err = ikTfList_step(&(self->errorTfList), err);

    /* apply gain schedule */
    if (NULL != self->gainSchedX) err = err * ikLutbl_eval(&(self->gainSched), *(self->gainSchedX));
    self->gainSchedOutput = err;
    
    /* take step on post-gain path */
    err = ikTfList_step(&(self->postGainTfList), err);

    return err;
}

int ikLinCon_getOutput(const ikLinCon *self, double *output, const char *name) {
    size_t blocklen;
    int index;
    const char *separator;
    
    /* declare error code */
    int err = -1;

    /* fetch signal values */
    if (!strcmp(name, "demand")) {
        *output = self->demand;
        return 0;
    }
    if (!strcmp(name, "filtered demand")) {
        *output = self->filteredDemand;
        return 0;
    }
    if (!strcmp(name, "measurement")) {
        *output = self->measurement;
        return 0;
    }
    if (!strcmp(name, "filtered measurement")) {
        *output = self->filteredMeasurement;
        return 0;
    }
    if (!strcmp(name, "error")) {
        *output = ikTfList_getOutput(&(self->demandTfList), 0) - ikTfList_getOutput(&(self->measurementTfList), 0);
        return 0;
    }
    if (!strcmp(name, "control action")) {
        *output = ikTfList_getOutput(&(self->errorTfList), 0);
        return 0;
    }
    if (!strcmp(name, "gain schedule")) {
        *output = self->gainSchedOutput;
        return 0;
    }
    if (!strcmp(name, "post-gain value")) {
        *output = self->gainSchedOutput;
        return 0;
    }

    /* get block name length and index for block */
    blocklen = strlen(name);
    index = 0;
    /* find the separator ">" */
    separator = strstr(name, ">");
    /* if there's no separator, leave the default values */
    if (NULL != separator) {
        /* if there's nothing after the separator, leave the default index value 0 */
        if (1 < strlen(separator)) {
            /* parse the index */
            index = atoi(separator + 1);
        }
        /* truncate name to leave only the block name */
        blocklen = strcspn(name, ">");
        /* change error code */
        err = -2;
    }

    /* fetch block values */
    if ((blocklen == strlen("post-gain transfer functions"))
            && !strncmp(name, "post-gain transfer functions", blocklen)) {
        *output = ikTfList_getOutput(&(self->postGainTfList), index);
        return 0;
    }
    if ((blocklen == strlen("demand transfer functions"))
            && !strncmp(name, "demand transfer functions", blocklen)) {
        *output = ikTfList_getOutput(&(self->demandTfList), index);
        return 0;
    }
    if ((blocklen == strlen("measurement transfer functions"))
            && !strncmp(name, "measurement transfer functions", blocklen)) {
        *output = ikTfList_getOutput(&(self->measurementTfList), index);
        return 0;
    }
    if ((blocklen == strlen("error transfer functions"))
            && !strncmp(name, "error transfer functions", blocklen)) {
        *output = ikTfList_getOutput(&(self->errorTfList), index);
        return 0;
    }
    if ((blocklen == strlen("demand notch filters"))
            && !strncmp(name, "demand notch filters", blocklen)) {
        *output = ikNotchList_getOutput(&(self->demandNotchList), index);
        return 0;
    }
    if ((blocklen == strlen("measurement notch filters"))
            && !strncmp(name, "measurement notch filters", blocklen)) {
        *output = ikNotchList_getOutput(&(self->measurementNotchList), index);
        return 0;
    }


    return err;
}


/* @endcond */
