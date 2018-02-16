//
// Created by Elijah on 18/12/2017.
//

#ifndef DEMORANGES_PARTICLEUTILS_H
#define DEMORANGES_PARTICLEUTILS_H

#include "../../structures/particle.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// If boolean is not correctly defined.
#if !defined(boolean)
#define boolean bool
#define TRUE true
#define FALSE false
#endif

#define PI 3.14159265358979323846

void printParticle(particle *p);

float get_particle_mass(particle *p);

boolean checkPositions(particle *particles, cl_ulong NUMPARTS, cl_float domain_length);

#endif //DEMORANGES_PARTICLEUTILS_H
