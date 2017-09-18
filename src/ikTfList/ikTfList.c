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
 * @file ikTfList.c
 * 
 * @brief Class ikTfList implementation
 */

/* @cond */

#include <stdlib.h>
#include "../ikTfList/ikTfList.h"

int ikTfList_init(ikTfList *self, const ikTfListParams *params) {
    /* initialise error code */
    int err = 0;
    int err_;
    /* initialise all transfer functions */
    int i;
    for (i = 0; i < IKTFLIST_NMAX; i++) {
        /* default initialisation */
        ikSlti_init(&(self->tfs[i]));
        self->enable[i] = 0;
        self->varEnable[i] = NULL;
        self->minInput[i] = NULL;
        self->maxInput[i] = NULL;
        self->minOutput[i] = NULL;
        self->maxOutput[i] = NULL;
        /* if not to be used, leave at that */
        if (!(params->tfParams[i].enable) && (NULL == params->tfParams[i].variableEnable)) continue;
        /* copy enable settings */
        self->enable[i] = params->tfParams[i].enable;
        self->varEnable[i] = params->tfParams[i].variableEnable;
        /* configure saturation */
        if (NULL == params->tfParams[i].minInput) {
            if (NULL == params->tfParams[i].maxInput) {
                ikSlti_setInSat(&(self->tfs[i]), 0, 0.0, 0.0);
            } else {
                ikSlti_setInSat(&(self->tfs[i]), 1, 0.0, 0.0);
            }
        } else {
            if (NULL == params->tfParams[i].maxInput) {
                ikSlti_setInSat(&(self->tfs[i]), -1, 0.0, 0.0);
            } else {
                ikSlti_setInSat(&(self->tfs[i]), 2, 0.0, 0.0);
            }
        }
        if (NULL == params->tfParams[i].minOutput) {
            if (NULL == params->tfParams[i].maxOutput) {
                ikSlti_setOutSat(&(self->tfs[i]), 0, 0.0, 0.0);
            } else {
                ikSlti_setOutSat(&(self->tfs[i]), 1, 0.0, 0.0);
            }
        } else {
            if (NULL == params->tfParams[i].maxOutput) {
                ikSlti_setOutSat(&(self->tfs[i]), -1, 0.0, 0.0);
            } else {
                ikSlti_setOutSat(&(self->tfs[i]), 2, 0.0, 0.0);
            }
        }
        /* copy addresses of saturation limits */
        self->minInput[i] = params->tfParams[i].minInput;
        self->maxInput[i] = params->tfParams[i].maxInput;
        self->minOutput[i] = params->tfParams[i].minOutput;
        self->maxOutput[i] = params->tfParams[i].maxOutput;
        /* set transfer function parameters */
        err_ = ikSlti_setParam(&(self->tfs[i]), params->tfParams[i].a, params->tfParams[i].b);
        if (!err && err_) err = -(i + 1);   
    }
    
    /* return error */
    return err;
}

void ikTfList_initParams(ikTfListParams *params) {
    int j;
        
    /* go through all the transfer functions */
    int i;
    for (i = 0; i < IKTFLIST_NMAX; i++) {
        /* set transfer function parameters to static gain of 1 */
        params->tfParams[i].a[0] = 1.0;
        params->tfParams[i].b[0] = 1.0;
        for (j = 1; j < 3; j++) {
            params->tfParams[i].a[j] = 0.0;
            params->tfParams[i].b[j] = 0.0;
        }
        /* permanently disable the transfer function */
        params->tfParams[i].enable = 0;
        params->tfParams[i].variableEnable = NULL;
        /* disable saturation */
        params->tfParams[i].minInput = NULL;
        params->tfParams[i].maxInput = NULL;
        params->tfParams[i].minOutput = NULL;
        params->tfParams[i].maxOutput = NULL;
    }
}

double ikTfList_step(ikTfList *self, double input) { 
    /* declare output and intermediate signals */
    double output = input;
    double output_;
    /* repeat for all the transfer functions */
    int i;
    for (i = IKTFLIST_NMAX - 1; i >= 0; i--) {
        /* pick up the saturation limits */
        int sat;
        double minsat;
        double maxsat;
        sat = ikSlti_getInSat(&(self->tfs[i]), &minsat, &maxsat);
        if (NULL != self->minInput[i]) minsat = *(self->minInput[i]);
        if (NULL != self->maxInput[i]) maxsat = *(self->maxInput[i]);
        ikSlti_setInSat(&(self->tfs[i]), sat, minsat, maxsat);
        sat = ikSlti_getOutSat(&(self->tfs[i]), &minsat, &maxsat);
        if (NULL != self->minOutput[i]) minsat = *(self->minOutput[i]);
        if (NULL != self->maxOutput[i]) maxsat = *(self->maxOutput[i]);
        ikSlti_setOutSat(&(self->tfs[i]), sat, minsat, maxsat);
        
        /* run a step */
        output_ = ikSlti_step(&(self->tfs[i]), output);
        
        /* if enabled, pick up output */
        if (NULL != self->varEnable[i]) self->enable[i] = *(self->varEnable[i]);
        if (self->enable[i]) output = output_;
    }
    
    /* return output */
    return output;
}

double ikTfList_getOutput(const ikTfList *self, int index) {
    /* saturate index */
    int index_ = index;
    if (0 > index_) index_ = 0;
    if (IKTFLIST_NMAX - 1 < index_) index_ = IKTFLIST_NMAX - 1;
    
    /* return corresponding output */
    return ikSlti_getOutput(&(self->tfs[index_]));
}



/* @endcond */
