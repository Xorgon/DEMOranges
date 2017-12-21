//
// Created by Elijah on 18/12/2017.
//

#ifndef DEMORANGES_PARTICLEUTILS_H
#define DEMORANGES_PARTICLEUTILS_H

#include "../structures/particle.h"
#include <stdio.h>
#include <math.h>

void printParticle(particle *p);

void writeParticles(particle *particles, float time, char *prefix, char *dir, cl_ulong NUMPART);

#endif //DEMORANGES_PARTICLEUTILS_H
