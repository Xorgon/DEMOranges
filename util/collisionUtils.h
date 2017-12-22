//
// Created by Elijah on 22/12/2017.
//

#ifndef DEMORANGES_COLLISIONUTILS_H
#define DEMORANGES_COLLISIONUTILS_H

#include "particleUtils.h"

float get_reduced_particle_mass(particle *p1, particle *p2);

float get_damping_coefficient(float restitution_coeff, float stiffness, float reduced_particle_mass);

#endif //DEMORANGES_COLLISIONUTILS_H
