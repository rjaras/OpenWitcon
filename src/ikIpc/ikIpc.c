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

#include <math.h>
#include <string.h>
#include "ikIpc.h"

/**
 * @file ikIpc.c
 * 
 * @brief Class ikIpc implementation
 */

/* @cond */

int ikIpc_init(ikIpc *self, const ikIpcParams *params) {
    int i, j;
    int err;
    int err_ = 0;
    
    /* Initialise inputs */
    self->in.azimuth = 0.0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            self->in.bladeRootMoments[i].c[j] = 0.0;
        }
    }
    self->in.collectivePitch = 0.0;
    self->in.demandedMy = 0.0;
    self->in.demandedMz = 0.0;
    self->in.externalPitchY = 0.0;
    self->in.externalPitchZ = 0.0;
    self->in.maximumIndividualPitch = 0.0;
    self->in.maximumPitch = 0.0;
    self->in.minimumPitch = 0.0;
    
    /* Calculate azimuth offsets */
    for (i = 0; i < 3; i++) {
        self->priv.azimuthOffsets[i] = params->azimuthOffset/180.0*3.14159265358979 + params->bladeOrder * i * 2.0/3.0*3.14159265358979;
    }
    
    /* Initialise control loops */
    err = ikConLoop_init(&(self->priv.conMy), &(params->controlMy));
    if (err && !err_) err_ = -1;
    
    err = ikConLoop_init(&(self->priv.conMz), &(params->controlMz));
    if (err && !err_) err_ = -2;
    
    return err_;
}

void ikIpc_initParams(ikIpcParams *params) {
    
    /* set defaults */
    params->azimuthOffset = 0.0;
    params->bladeOrder = 1;
    
    /* initialise member parameters */
    ikConLoop_initParams(&(params->controlMy));
    ikConLoop_initParams(&(params->controlMz));
}

void ikIpc_step(ikIpc *self) {
    int i;
    ikVector rotations[3];
    ikVector pitchAxes[3];
    double pitchMarginUp, pitchMarginDown;
    
    /* Transform rotating frame blade root moments to non-rotating frame and add them up */
    /* Also transform the rotating frame z axes to non-rotating frame*/
    for (i = 0; i < 3; i++) {
        self->priv.staticMoment.c[i] = 0.0;
    }
    for (i = 0; i < 3; i++) {
        rotations[i].c[0] = fmod(self->in.azimuth/180.0*3.14159265358979 + self->priv.azimuthOffsets[i], 2.0*3.14159265358979);
        rotations[i].c[1] = 0.0;
        rotations[i].c[2] = 0.0;
        self->priv.staticMoment = ikVector_add( self->priv.staticMoment, ikVector_rotate(self->in.bladeRootMoments[i], rotations[i]) );
        pitchAxes[i].c[0] = 0.0;
        pitchAxes[i].c[1] = 0.0;
        pitchAxes[i].c[2] = 1.0;
        pitchAxes[i] = ikVector_rotate(pitchAxes[i], rotations[i]);
    }
    
    /* calculate maximum pitch increment module */
    pitchMarginUp = self->in.maximumPitch - self->in.collectivePitch;
    pitchMarginUp = pitchMarginUp > 0.0 ? pitchMarginUp : 0.0;
    pitchMarginDown = self->in.minimumPitch - self->in.collectivePitch;
    pitchMarginDown = pitchMarginDown < 0.0 ? pitchMarginDown : 0.0;
    self->priv.maxPitchIncrementMod = fabs(pitchMarginUp) < fabs(pitchMarginDown) ? fabs(pitchMarginUp) : fabs(pitchMarginDown);
    self->priv.maxPitchIncrementMod = self->priv.maxPitchIncrementMod < self->in.maximumIndividualPitch ? self->priv.maxPitchIncrementMod : self->in.maximumIndividualPitch;
    
    /* figure out the limits for the My control loop */
    self->priv.maxPitchZ = self->priv.maxPitchIncrementMod*self->priv.maxPitchIncrementMod - self->priv.pitchYcon*self->priv.pitchYcon;
    self->priv.maxPitchZ  = self->priv.maxPitchZ > 0.0 ? self->priv.maxPitchZ : 0.0;
    self->priv.maxPitchZ  = sqrt(self->priv.maxPitchZ);
    
    /* run My control loop */
    self->priv.pitchZcon = ikConLoop_step(&(self->priv.conMy), self->in.demandedMy, self->priv.staticMoment.c[1], -self->priv.maxPitchZ, self->priv.maxPitchZ);
    
    /* figure out the limits for the Mz control loop */
    self->priv.maxPitchY = self->priv.maxPitchIncrementMod*self->priv.maxPitchIncrementMod - self->priv.pitchZcon*self->priv.pitchZcon;
    self->priv.maxPitchY  = self->priv.maxPitchY > 0.0 ? self->priv.maxPitchY : 0.0;
    self->priv.maxPitchY  = sqrt(self->priv.maxPitchY);
    
    /* Run My and Mz control loops */
    self->priv.pitchYcon = ikConLoop_step(&(self->priv.conMz), self->in.demandedMz, self->priv.staticMoment.c[2], -self->priv.maxPitchY, self->priv.maxPitchY);
    
    /* add the external pitch actions */
    self->priv.staticPitch.c[0] = 0.0;
    self->priv.staticPitch.c[2] = self->in.externalPitchZ + self->priv.pitchZcon;
    self->priv.staticPitch.c[1] = self->in.externalPitchY - self->priv.pitchYcon;
    
    /* Transform the non-rotating frame pitch angle to rotating frame, */
    /* then add the indivitual pitch angles to the collective one, and */
    /* saturate. */
    for (i = 0; i < 3; i++) {
        self->priv.pitchDifferentials[i] = ikVector_dot(self->priv.staticPitch, pitchAxes[i]);
        self->out.pitch[i] = self->in.collectivePitch + self->priv.pitchDifferentials[i];
    }

}

int ikIpc_getOutput(const ikIpc *self, double *output, const char *name) {
    int err;
    const char *sep;
    
    /* pick up the signal names */
    if (!strcmp(name, "My")) {
        *output = self->priv.staticMoment.c[1];
        return 0;
    }
    if (!strcmp(name, "Mz")) {
        *output = self->priv.staticMoment.c[2];
        return 0;
    }
    if (!strcmp(name, "pitch y from control")) {
        *output = self->priv.pitchYcon;
        return 0;
    }
    if (!strcmp(name, "pitch z from control")) {
        *output = self->priv.pitchZcon;
        return 0;
    }
    if (!strcmp(name, "pitch y")) {
        *output = self->priv.staticPitch.c[1];
        return 0;
    }
    if (!strcmp(name, "pitch z")) {
        *output = self->priv.staticPitch.c[2];
        return 0;
    }
    if (!strcmp(name, "pitch increment 1")) {
        *output = self->priv.pitchDifferentials[0];
        return 0;
    }
    if (!strcmp(name, "pitch increment 2")) {
        *output = self->priv.pitchDifferentials[1];
        return 0;
    }
    if (!strcmp(name, "pitch increment 3")) {
        *output = self->priv.pitchDifferentials[2];
        return 0;
    }

    /* pick up the block names */
    sep = strstr(name, ">");
    if (NULL == sep) return -1;
    if (!strncmp(name, "My control", strlen(name) - strlen(sep))) {
        err = ikConLoop_getOutput(&(self->priv.conMy), output, sep + 1);
        if (err) return -1;
        else return 0;
    }
    if (!strncmp(name, "Mz control", strlen(name) - strlen(sep))) {
        err = ikConLoop_getOutput(&(self->priv.conMz), output, sep + 1);
        if (err) return -1;
        else return 0;
    }

    return -2;
}

/* @endcond */
