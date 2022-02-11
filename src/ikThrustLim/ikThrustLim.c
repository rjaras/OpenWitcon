/*
  Copyright (C) 2021-2022 IKERLAN
  
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
 * @file ikThrustLim.c
 * 
 * @brief Class ikThrustLim implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ikThrustLim.h"

char* ikThrustLim_init(ikThrustLim *self, const ikThrustLimParams *params) {
    const char* errStr = "";
    char* outputErrorMessage = malloc(100 * sizeof(char));
	
    /*pass parameters on*/
    self->rho = params->rho;
    self->R = params->R;
    self->ctlambda2Max = params->ctlambda2Max;
    self->ctlambda2Min = params->ctlambda2Min;
    self->minPitchMaxChangeRate = params->minPitchMaxChangeRate;
    self->samplingInterval = params->samplingInterval;
    
    /*construct ct/lambda^2 surface*/
    errStr = ikSurf_newf(&(self->surfCtlambda2), params->ctlambda2SurfaceFileName);
    if (strlen(errStr)) {
        snprintf(outputErrorMessage, 100, "Error initializing ct/lambda^2 surface: %s", errStr);
        return outputErrorMessage;
    }

    return "";
}

void ikThrustLim_initParams(ikThrustLimParams *params) {
    params->rho = 1.0;
    params->R = 1.0;
    params->ctlambda2SurfaceFileName = "ctlambda2.bin";
    params->ctlambda2Max = 1E3;
    params->ctlambda2Min = -1E3;
}

double ikThrustLim_step(ikThrustLim *self, double tipSpeedRatio, double rotorSpeed, double maximumThrust) {
    double x[2];
    
    self->rotorSpeed = rotorSpeed;
    self->maximumThrust = maximumThrust;
    self->tipSpeedRatio = tipSpeedRatio;
    self->ctlambda2 = 2 * maximumThrust * 1E3 / (self->rho * 3.1415926536 * self->R*self->R*self->R*self->R * rotorSpeed*rotorSpeed);
    x[0] = tipSpeedRatio;
    x[1] = self->ctlambda2;
    double newMinimumPitch = ikSurf_eval(self->surfCtlambda2, 1, x, 1);
    
    // Limit the value of the minimum pitch
    newMinimumPitch = (self->ctlambda2 > self->ctlambda2Max ? 0.0 : newMinimumPitch);
    newMinimumPitch = (self->ctlambda2 < self->ctlambda2Min ? 0.0 : newMinimumPitch);
    newMinimumPitch = (newMinimumPitch < 0.0 ? 0.0 : newMinimumPitch);
    double minPitchChange = newMinimumPitch - self->minimumPitch;
    if (minPitchChange > self->minPitchMaxChangeRate * self->samplingInterval) {
        self->minimumPitch = self->minimumPitch + self->minPitchMaxChangeRate * self->samplingInterval;
    } else if (minPitchChange < - self->minPitchMaxChangeRate * self->samplingInterval) {
        self->minimumPitch = self->minimumPitch - self->minPitchMaxChangeRate * self->samplingInterval;
    } else {
        self->minimumPitch = newMinimumPitch;
    }

    // Limit the value for ctlambda2
    self->ctlambda2 = (self->ctlambda2 > self->ctlambda2Max ? self->ctlambda2Max : self->ctlambda2);
    self->ctlambda2 = (self->ctlambda2 < self->ctlambda2Min ? self->ctlambda2Min : self->ctlambda2);
    
    return self->minimumPitch;
}

int ikThrustLim_getOutput(const ikThrustLim *self, double *output, const char *name) {
    /* pick up the signal names */
    if (!strcmp(name, "rotor speed")) {
        *output = self->rotorSpeed;
        return 0;
    }
    if (!strcmp(name, "tip-speed ratio")) {
        *output = self->tipSpeedRatio;
        return 0;
    }
    if (!strcmp(name, "maximum thrust")) {
        *output = self->maximumThrust;
        return 0;
    }
    if (!strcmp(name, "Ct/lambda^2")) {
        *output = self->ctlambda2;
        return 0;
    }
    if (!strcmp(name, "minimum pitch")) {
        *output = self->minimumPitch;
        return 0;
    }

    return -1;
}

void ikThrustLim_delete(ikThrustLim *self) {
    ikSurf_delete(self->surfCtlambda2);
}
