//
// Created by Elijah on 30/11/2017.
//

#ifndef DEMORANGES_PARTICLE_H
#define DEMORANGES_PARTICLE_H

#include <CL/cl_platform.h>

typedef struct {
    cl_ulong particle_id;
    cl_float particle_diameter;
    cl_float density;
    cl_float *fluid_viscosity;

    cl_float3 pos;
    cl_float3 vel;
    cl_float3 forces;
} particle;

#endif //DEMORANGES_PARTICLE_H
