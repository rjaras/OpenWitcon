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

#include <stdlib.h>
#include <string.h>
#include "ikConLoop.h"

/**
 * @file ikConLoop.c
 * 
 * @brief Class ikConLoop implementation
 */

/* @cond */

int ikConLoop_init(ikConLoop *self, const ikConLoopParams *params) {
    int err = 0;
    int err_;
    
    /* declare tweaked parameter structure */
    ikLinConParams linconparams = params->linearController;

    /* tweak the linear controller parameters with pointers to the control */
    /* action limits */
    linconparams.maxControlAction = &(self->maximumControlAction);
    linconparams.minControlAction = &(self->minimumControlAction);

    /* tweak the linear controller parameters with a pointer to the selected */
    /* preset, but only if regions have been defined */
    if (0 < params->regionSelector.nRegions)
        linconparams.config = &(self->selectedRegion);

    /* invoke component initialisation */
    err_ = ikStpgen_init(&(self->stpgen), &(params->setpointGenerator));
    if (!err && err_) err = -1;
    err_ = ikLinCon_init(&(self->lincon), &linconparams);
    if (!err && err_) err = -2;
    err_ = ikLinCon_init(&(self->setpointFilters), &(params->setpointFilters));
    if (!err && err_) err = -3;
    err_ = ikLinCon_init(&(self->controlActionFilters), &(params->controlActionFilters));
    if (!err && err_) err = -4;
    err_ = ikRegionSelector_init(&(self->regionSelector), &(params->regionSelector));
    if (!err && err_) err = -5;


    /* initialise members */
    self->controlAction = 0.0;
    self->maximumControlAction = 0.0;
    self->minimumControlAction = 0.0;
    self->selectedRegion = 0;
    self->x = 0.0;
    self->y = 0.0;

    return err;
}

void ikConLoop_initParams(ikConLoopParams *params) {
    int i;
    
    /* invoke component initialisation */
    ikRegionSelector_initParams(&(params->regionSelector));
    ikLinCon_initParams(&(params->setpointFilters));
    ikLinCon_initParams(&(params->controlActionFilters));
    ikLinCon_initParams(&(params->linearController));
    ikStpgen_initParams(&(params->setpointGenerator));

    /* set measurement gain to 0.0 in filters for region selector */
    params->setpointFilters.measurementTfs.tfParams[0].enable = 1;
    params->setpointFilters.measurementTfs.tfParams[0].b[0] = 0.0;
    params->controlActionFilters.measurementTfs.tfParams[0].enable = 1;
    params->controlActionFilters.measurementTfs.tfParams[0].b[0] = 0.0;

    /* enable last transfer function of linear controller in all configurations */
    for (i = 0; i < IKLINCON_MAXNCONFIG; i++)
        params->linearController.errorTfsEnable[i][0] = 1;
}

double ikConLoop_step(ikConLoop *self, double maxSp, double feedback, double minCon, double maxCon) {
    /* take step with setpoint generator */
    double setpoint = ikStpgen_step(&(self->stpgen), maxSp, feedback, self->controlAction, minCon, maxCon);

    /* take step with setpoint and control action filters */
    self->x = ikLinCon_step(&(self->setpointFilters), setpoint, self->controlAction);
    self->y = ikLinCon_step(&(self->controlActionFilters), self->controlAction, setpoint);

    /* select a preset */
    self->selectedRegion = ikRegionSelector_getRegion(&(self->regionSelector), self->x, self->y);

    /* set saturation limits */
    ikStpgen_getOutput(&(self->stpgen), &(self->minimumControlAction), "minimum control action");
    ikStpgen_getOutput(&(self->stpgen), &(self->maximumControlAction), "maximum control action");

    /* take step with linear controller */
    self->controlAction = ikLinCon_step(&(self->lincon), setpoint, feedback);

    return self->controlAction;
}

int ikConLoop_getOutput(const ikConLoop *self, double *output, const char *name) {
    int err;
    const char *sep;
    
    /* pick up the signal names */
    if (!strcmp(name, "control action")) {
        *output = self->controlAction;
        return 0;
    }
    if (!strcmp(name, "setpoint")) {
        ikStpgen_getOutput(&(self->stpgen), output, "setpoint");
        return 0;
    }
    if (!strcmp(name, "minimum control action")) {
        ikStpgen_getOutput(&(self->stpgen), output, "minimum control action");
        return 0;
    }
    if (!strcmp(name, "maximum control action")) {
        ikStpgen_getOutput(&(self->stpgen), output, "maximum control action");
        return 0;
    }
    if (!strcmp(name, "external minimum control action")) {
        ikStpgen_getOutput(&(self->stpgen), output, "external minimum control action");
        return 0;
    }
    if (!strcmp(name, "external maximum control action")) {
        ikStpgen_getOutput(&(self->stpgen), output, "external maximum control action");
        return 0;
    }
    if (!strcmp(name, "maximum setpoint")) {
        ikStpgen_getOutput(&(self->stpgen), output, "external maximum setpoint");
        return 0;
    }
    if (!strcmp(name, "feedback")) {
        ikStpgen_getOutput(&(self->stpgen), output, "feedback");
        return 0;
    }
    if (!strcmp(name, "x")) {
        *output = self->x;
        return 0;
    }
    if (!strcmp(name, "y")) {
        *output = self->y;
        return 0;
    }
    if (!strcmp(name, "preset selection")) {
        *output = self->selectedRegion;
        return 0;
    }

    /* pick up the block names */
    sep = strstr(name, ">");
    if (NULL == sep) return -1;
    if (!strncmp(name, "setpoint generator", strlen(name) - strlen(sep))) {
        err = ikStpgen_getOutput(&(self->stpgen), output, sep + 1);
        if (err) return -1;
        else return 0;
    }
    if (!strncmp(name, "linear controller", strlen(name) - strlen(sep))) {
        err = ikLinCon_getOutput(&(self->lincon), output, sep + 1);
        if (err) return -1;
        else return 0;
    }
    if (!strncmp(name, "setpoint filters", strlen(name) - strlen(sep))) {
        err = ikLinCon_getOutput(&(self->setpointFilters), output, sep + 1);
        if (err) return -1;
        else return 0;
    }
    if (!strncmp(name, "control action filters", strlen(name) - strlen(sep))) {
        err = ikLinCon_getOutput(&(self->controlActionFilters), output, sep + 1);
        if (err) return -1;
        else return 0;
    }

    return -2;
}

/* @endcond */
