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

#include <string.h>

#include "ikThrustLim.h"

int ikThrustLim_init(ikThrustLim *self, const ikThrustLimParams *params) {
    const char* errStr = "";
	
    /*pass parameters on*/
    self->rho = params->rho;
    self->R = params->R;
    
    errStr = ikSurf_newf(&(self->surfCtlambda2), params->ctlambda2SurfaceFileName);
    if (strlen(errStr)) return -1;

    return 0;
}

void ikThrustLim_initParams(ikThrustLimParams *params) {
    params->rho = 1.0;
    params->R = 1.0;
    params->ctlambda2SurfaceFileName = "ctlambda2.bin";
}

double ikThrustLim_step(ikThrustLim *self, double tipSpeedRatio, double rotorSpeed, double maximumThrust) {
    double x[2];
    
    self->rotorSpeed = rotorSpeed;
    self->maximumThrust = maximumThrust;
    self->tipSpeedRatio = tipSpeedRatio;
    self->ctlambda2 = 2 * maximumThrust * 1E3 / (self->rho * 3.1415926536 * self->R*self->R*self->R*self->R * rotorSpeed*rotorSpeed);
    x[0] = tipSpeedRatio;
    x[1] = self->ctlambda2;
    self->minimumPitch = ikSurf_eval(self->surfCtlambda2, 1, x, 1);
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
