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
 * @file ikTsrEst.c
 * 
 * @brief Class ikTsrEst implementation
 */

#include <stdlib.h>
#include <string.h>

#include "ikTsrEst.h"

int ikTsrEst_init(ikTsrEst *self, const ikTsrEstParams *params) {
    int i, err;
	const char* errStr = "";
	
    /*allocate parameters for derivation*/
    ikTfListParams derParams;
    
    /*pass parameters on*/
    self->b = params->b;
    self->J = params->J;
    self->rho = params->rho;
    self->R = params->R;
	
    /*initialise filters*/
    err = ikNotchList_init(&(self->rotorSpeedNotchFilters), &(params->notches));
    if (err) return -1;
    err = ikNotchList_init(&(self->generatorTorqueNotchFilters), &(params->notches));
    if (err) return -2;
    err = ikNotchList_init(&(self->pitchAngleNotchFilters), &(params->notches));
    if (err) return -3;
    err = ikTfList_init(&(self->rotorSpeedLowPassFilter), &(params->lowPass));
    if (err) return -4;
    err = ikTfList_init(&(self->generatorTorqueLowPassFilter), &(params->lowPass));
    if (err) return -5;
    err = ikTfList_init(&(self->pitchAngleLowPassFilter), &(params->lowPass));
    if (err) return -6;

    /*add derivative to the low pass filter*/
    derParams = params->lowPass;
    for (i = 0; i < IKTFLIST_NMAX; i++) {
	if (!derParams.tfParams[i].enable) {
	    derParams.tfParams[i].enable = 1;
	    derParams.tfParams[i].b[0] = 2.0/params->T;
	    derParams.tfParams[i].b[1] = -2.0/params->T;
	    derParams.tfParams[i].b[2] = 0.0;
	    derParams.tfParams[i].a[0] = 1.0;
	    derParams.tfParams[i].a[1] = 1.0;
	    derParams.tfParams[i].a[2] = 0.0;
	    break;
	}
    }

    /*initialise derivative*/
    err = ikTfList_init(&(self->rotorSpeedDerivation), &(derParams));
    if (err) return -7;

    /*construct cp/lambda^3 surface*/
    errStr = ikSurf_newf(&(self->surfCplambda3), params->cplambda3SurfaceFileName);
    if (strlen(errStr)) return -8;
	
    return 0;
}

void ikTsrEst_initParams(ikTsrEstParams *params) {
    params->b = 1.0;
    params->J = 1.0;
    params->rho = 1.0;
    params->R = 1.0;
    params->cplambda3SurfaceFileName = "cplambda3.bin";
    params->T = 0.01;
    
    ikNotchList_initParams(&(params->notches));
    ikTfList_initParams(&(params->lowPass));
}

double ikTsrEst_step(ikTsrEst *self, double generatorSpeed, double generatorTorque, double pitchAngle) {
    double aux;
    double x[2];
    
    self->unfilteredRotorSpeed = generatorSpeed / self->b;
    self->pitchAngle = pitchAngle;
    aux = ikNotchList_step(&(self->rotorSpeedNotchFilters), self->unfilteredRotorSpeed);
    self->rotorSpeed = ikTfList_step(&(self->rotorSpeedLowPassFilter), aux);
    self->rotorAcceleration = ikTfList_step(&(self->rotorSpeedDerivation), aux);
    aux = ikNotchList_step(&(self->pitchAngleNotchFilters), pitchAngle);
    self->filteredPitchAngle = ikTfList_step(&(self->pitchAngleLowPassFilter), aux);
    aux = ikNotchList_step(&(self->generatorTorqueNotchFilters), generatorTorque);
    self->filteredGeneratorTorque = ikTfList_step(&(self->generatorTorqueLowPassFilter), aux) * 1E3; // kNm -> Nm
    self->aerodynamicTorque = self->J * self->rotorAcceleration + self->b * self->filteredGeneratorTorque;
    self->cplambda3 = 2 * self->aerodynamicTorque / (self->rho * 3.1415926536 * self->R*self->R*self->R*self->R*self->R * self->rotorSpeed*self->rotorSpeed);
    x[0] = self->filteredPitchAngle;
    x[1] = self->cplambda3;
    self->tipSpeedRatio = ikSurf_eval(self->surfCplambda3, 0, x, 1);
    return self->tipSpeedRatio;
}

int ikTsrEst_getOutput(const ikTsrEst *self, double *output, const char *name) {
    size_t blocklen;
    int index;
    int err;
    const char *separator;

    /* pick up the signal names */
    if (!strcmp(name, "tip-speed ratio")) {
        *output = self->tipSpeedRatio;
        return 0;
    }
    if (!strcmp(name, "filtered pitch angle")) {
        *output = self->filteredPitchAngle;
        return 0;
    }
    if (!strcmp(name, "pitch angle")) {
        *output = self->pitchAngle;
        return 0;
    }
    if (!strcmp(name, "rotor speed")) {
        *output = self->rotorSpeed;
        return 0;
    }
    if (!strcmp(name, "aerodynamic torque")) {
        *output = self->aerodynamicTorque;
        return 0;
    }
    if (!strcmp(name, "rotor acceleration")) {
        *output = self->rotorAcceleration;
        return 0;
    }
    if (!strcmp(name, "filtered generator torque")) {
        *output = self->filteredGeneratorTorque;
        return 0;
    }
    if (!strcmp(name, "unfiltered rotor speed")) {
        *output = self->unfilteredRotorSpeed;
        return 0;
    }
    if (!strcmp(name, "generator speed")) {
        *output = self->generatorSpeed;
        return 0;
    }
    if (!strcmp(name, "generator torque")) {
        *output = self->generatorTorque;
        return 0;
    }
    if (!strcmp(name, "Cp/lambda^3")) {
        *output = self->cplambda3;
        return 0;
    }

    /* pick up the block names */
    separator = strstr(name, ">");
    if (NULL == separator) return -1;
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
    if ((blocklen == strlen("pitch angle low pass filter"))
            && !strncmp(name, "pitch angle low pass filter", blocklen)) {
        *output = ikTfList_getOutput(&(self->pitchAngleLowPassFilter), index);
        return 0;
    }
    if ((blocklen == strlen("pitch angle notch filters"))
            && !strncmp(name, "pitch angle notch filters", blocklen)) {
        *output = ikNotchList_getOutput(&(self->pitchAngleNotchFilters), index);
        return 0;
    }
    if ((blocklen == strlen("generator torque low pass filter"))
            && !strncmp(name, "generator torque low pass filter", blocklen)) {
        *output = ikTfList_getOutput(&(self->generatorTorqueLowPassFilter), index);
        return 0;
    }
    if ((blocklen == strlen("generator torque notch filters"))
            && !strncmp(name, "generator torque notch filters", blocklen)) {
        *output = ikNotchList_getOutput(&(self->generatorTorqueNotchFilters), index);
        return 0;
    }
    if ((blocklen == strlen("rotor speed low pass filter"))
            && !strncmp(name, "rotor speed low pass filter", blocklen)) {
        *output = ikTfList_getOutput(&(self->rotorSpeedLowPassFilter), index);
        return 0;
    }
    if ((blocklen == strlen("rotor speed notch filters"))
            && !strncmp(name, "rotor speed notch filters", blocklen)) {
        *output = ikNotchList_getOutput(&(self->rotorSpeedNotchFilters), index);
        return 0;
    }
    if ((blocklen == strlen("rotor speed derivation"))
            && !strncmp(name, "rotor speed derivation", blocklen)) {
        *output = ikTfList_getOutput(&(self->rotorSpeedDerivation), index);
        return 0;
    }

    return -2;
}

void ikTsrEst_delete(ikTsrEst *self) {
    ikSurf_delete(self->surfCplambda3);
}
