//
// Created by Elijah on 18/12/2017.
//

#ifndef DEMORANGES_PARTICLEUTILS_H
#define DEMORANGES_PARTICLEUTILS_H

#include "../../structures/particle.h"
#include <stdio.h>
#include <math.h>
#if defined(_MSC_VER)
#include <Windows.h>
#endif
#include <stdbool.h>
#include <stdlib.h>

// If boolean is not correctly defined.
#if !defined(boolean)
#define boolean bool
#if !defined(_MSC_VER)
#define TRUE true
#define FALSE false
#endif
#endif

#define PI 3.14159265358979323846

void printParticle(particle *p);

float get_particle_mass_from_values(float density, float diameter);

float get_particle_mass(particle *p);

float get_tau(particle *p);

void initializeMonodisperseParticles(particle *particles, cl_ulong NUMPART, float density, float fluid_viscosity,
                                     float diameter, float effect_diameter, cl_float3 positions[],
                                     cl_float3 velocities[]);

/**
 * Populates an array of positions with position vectors arranged in a cube shape.
 * @param positions cl_float3 array of length NUMPART.
 * @param NUMPART Number of particles.
 * @param particle_diameter Diameter of particles, or maximum diameter if polydisperse.
 * @param spacing_factor A factor describing the spacing between particles.
 * @return Length of cube.
 */
float createCubePositions(cl_float3 *positions, cl_ulong NUMPART, float particle_diameter, float spacing_factor, cl_float3 center);

void createNormalDistVelocities(cl_float3 *velocities, cl_ulong NUMPART, float mean, float std_dev);

boolean checkPositions(particle *particles, cl_ulong NUMPARTS, cl_float domain_length);

#endif //DEMORANGES_PARTICLEUTILS_H
