//
// Created by Elijah on 04/12/2017.
//

#ifndef DEMORANGES_WALL_H
#define DEMORANGES_WALL_H

#include <CL/cl_platform.h>

typedef struct {
    cl_float3 normal;
    cl_float3 max;
    cl_float3 min;
} aa_wall;

#endif //DEMORANGES_WALL_H
