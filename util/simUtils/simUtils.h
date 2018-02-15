//
// Created by Elijah on 15/02/2018.
//

#ifndef DEMORANGES_SIMUTILS_H
#define DEMORANGES_SIMUTILS_H

#include "../../structures/particle.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

void writeParticles(particle *particles, float time, char *prefix, char *dir, cl_ulong NUMPART);

void writeSetupData(char prefix[], char dir[], cl_ulong NUMPART, cl_float timestep, cl_float sim_length,
                    cl_float domain_length, cl_float stiffness, cl_float restitution_coefficient,
                    cl_float friction_coefficient, cl_float friction_stiffness, cl_float cohesion_stiffness);

void writeTime(char prefix[], char dir[], cl_ulong NUMPART, char label[]);

#endif //DEMORANGES_SIMUTILS_H
