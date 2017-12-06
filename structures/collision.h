//
// Created by Elijah on 04/12/2017.
//

#ifndef DEMORANGES_COLLISION_H
#define DEMORANGES_COLLISION_H

#include "particle.h"

typedef struct {
    particle p1;
    particle p2;

    cl_float stiffness;
    cl_float damping_coefficient;
    cl_float friction_coefficient;
    cl_float friction_stiffness;
} particle_collision;


#endif //DEMORANGES_COLLISION_H
