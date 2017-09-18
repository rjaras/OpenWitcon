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
 * @file ikNotchList.c
 * 
 * @brief Class ikNotchList implementation
 */


/* @cond */

#include <stdlib.h>
#include "../ikNotchList/ikNotchList.h"

int ikNotchList_init(ikNotchList *self, const struct ikNotchListParams *params) {
    /* declare error code */
    int err = 0;
    int err_;
    /* repeat for all the notch filters */
    int i;
    for (i = 0; i < IKNOTCHLIST_NMAX; i++) {
        /* copy enable settings */
        self->enable[i] = params->notchParams[i].enable;
        self->variableEnable[i] = params->notchParams[i].variableEnable;
        /* copy frequency settings */
        self->variableFreq[i] = params->notchParams[i].variableFreq;
        /* initialise notch filter */
        err_ = ikVfnotch_init(
                &(self->notches[i]),
                params->dT,
                params->notchParams[i].freq,
                params->notchParams[i].dampDen,
                params->notchParams[i].dampNum);

        /* register error code */
        if (-1 == err_) err = 1;
        if (!err && err_) err = -(i + 1);
    }

    /* return error code */
    return err;
}

void ikNotchList_initParams(struct ikNotchListParams *params) {
    int i;
    
    /* set sampling time to 1 */
    params->dT = 1.0;
    /* repeat for all the notch filters */
    for (i = 0; i < IKNOTCHLIST_NMAX; i++) {
        /* permanently disable the notch filter */
        params->notchParams[i].enable = 0;
        params->notchParams[i].variableEnable = NULL;
        /* permanently set frequency to 1 */
        params->notchParams[i].freq = 1.0;
        params->notchParams[i].variableFreq = NULL;
        /* set damping to 1 */
        params->notchParams[i].dampDen = 1.0;
        params->notchParams[i].dampNum = 1.0;
    }
}

double ikNotchList_step(ikNotchList *self, double input) {
    /* declare output */
    double output = input;
    double output_;
    /* repeat for every notch filter, backwards */
    int i;
    for (i = IKNOTCHLIST_NMAX - 1; i >= 0; i--) {
        /* set frequency */
        if (NULL != self->variableFreq[i]) ikVfnotch_setFreq(&(self->notches[i]), *(self->variableFreq[i]));
        /* take a step */
        output_ = ikVfnotch_step(&(self->notches[i]), output);
        /* if enabled, register output */
        if (NULL != self->variableEnable[i]) self->enable[i] = *(self->variableEnable[i]);
        if (self->enable[i]) output = output_;
    }

    /* return output */
    return output;
}

double ikNotchList_getOutput(const ikNotchList *self, int index) {
    /* saturate the index */
    int index_ = index;
    if (index_ < 0) index_ = 0;
    if (index_ > IKNOTCHLIST_NMAX - 1) index_ = IKNOTCHLIST_NMAX - 1;
    
    /* return the output of the corresponding notch filter */
    return ikVfnotch_getOutput(&(self->notches[index_]));
}

/* @endcond */
