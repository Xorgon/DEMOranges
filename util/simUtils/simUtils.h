//
// Created by Elijah on 15/02/2018.
//

#ifndef DEMORANGES_SIMUTILS_H
#define DEMORANGES_SIMUTILS_H

#include "../../structures/particle.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

// If boolean is not correctly defined.
#if !defined(boolean)
#define boolean bool
#if !defined(_MSC_VER)
#define TRUE true
#define FALSE false
#endif
#endif

void writeParticles(particle *particles, float time, char *prefix, char *dir, cl_ulong NUMPART, boolean log_vel);

void writeSetupData(char prefix[], char dir[], cl_ulong NUMPART, cl_float timestep, cl_float sim_length,
                    cl_float domain_length, cl_float stiffness, cl_float restitution_coefficient,
                    cl_float friction_coefficient, cl_float friction_stiffness, cl_float cohesion_stiffness,
                    cl_float particle_diameter, cl_float effect_diameter, cl_float particle_density,
                    cl_float fluid_viscosity);

void
writeCVStats(char prefix[], char dir[], cl_ulong NUMPART, cl_int *particle_count_array, cl_ulong NUMCVS, float time);

void writeTime(char prefix[], char dir[], cl_ulong NUMPART, char label[]);

void writeNumCols(char prefix[], char dir[], cl_ulong NUMCOLS, cl_ulong NUMPART, float time);

boolean checkDirExists(char dir[]);

float getFluidViscFromStks(float diameter, float density, float flow_speed, float length, float stks);

float getRestitutionFromSy(float eff_len, float k_c, float u, float mass, float sy);

float getCohesionFromSy(float eff_len, float restitution, float u, float mass, float sy);

#endif //DEMORANGES_SIMUTILS_H
