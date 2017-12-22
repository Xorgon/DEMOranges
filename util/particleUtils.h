//
// Created by Elijah on 18/12/2017.
//

#ifndef DEMORANGES_PARTICLEUTILS_H
#define DEMORANGES_PARTICLEUTILS_H

#include "../structures/particle.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

void printParticle(particle *p);

void writeParticles(particle *particles, float time, char *prefix, char *dir, cl_ulong NUMPART);

float get_particle_mass(particle *p);

#endif //DEMORANGES_PARTICLEUTILS_H
