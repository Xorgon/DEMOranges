//
// Created by Elijah on 18/12/2017.
//

#include "particleUtils.h"

void printParticle(particle *part) {
    particle p = *part;
    printf("%i\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", (int) p.id, p.diameter, p.density,
           p.fluid_viscosity, p.pos.x, p.pos.y, p.pos.z, p.vel.x, p.vel.y, p.vel.z, p.forces.x, p.forces.y, p.forces.z);
}

float get_particle_mass(particle *p) {
    return (float) ((*p).density * PI * powf((*p).diameter, 3) / 6);
}

boolean checkPositions(particle *particles, cl_ulong NUMPARTS, cl_float domain_length) {
    boolean correct = TRUE;
    for (cl_ulong i = 0; i < NUMPARTS; i++) {
        if (fabs(particles[i].pos.x) > domain_length / 2
            || fabs(particles[i].pos.y) > domain_length / 2
            || fabs(particles[i].pos.z) > domain_length / 2) {
            correct = FALSE;
        }
    }
    return correct;
}

void initializeMonodisperseParticles(particle *particles, cl_ulong NUMPART, float density, float fluid_viscosity,
                                     float diameter, float effect_diameter, cl_float3 positions[],
                                     cl_float3 velocities[]) {
    for (cl_ulong i = 0; i < NUMPART; i++) {
        particles[i].id = i;
        particles[i].density = density;
        particles[i].fluid_viscosity = fluid_viscosity;
        particles[i].diameter = diameter;
        particles[i].effect_diameter = effect_diameter;
        particles[i].pos = positions[i];
        if (velocities != NULL) {
            particles[i].vel = velocities[i];
        } else {
            particles[i].vel = (cl_float3) {0.0, 0.0, 0.0};
        }
        particles[i].forces = (cl_float3) {0.0, 0.0, 0.0};
    }
}

float createCubePositions(cl_float3 *positions, cl_ulong NUMPART, float particle_diameter) {
    cl_ulong cubert_NUMPART = (cl_ulong) ceil(pow(NUMPART, 0.334));
    cl_ulong pos_len = 0;
    srand(0);

    for (int x = 0; x < cubert_NUMPART; x++) {
        for (int y = 0; y < cubert_NUMPART; y++) {
            for (int z = 0; z < cubert_NUMPART; z++) {
                if (pos_len < NUMPART) {
                    float rand_offset = 0.005 * (float) rand() / (float) (RAND_MAX);
                    cl_float xf = 1.2 * cubert_NUMPART * particle_diameter
                                  * (-0.5 + rand_offset + ((float) x / cubert_NUMPART));
                    cl_float yf = 1.2 * cubert_NUMPART * particle_diameter
                                  * (-0.5 + rand_offset + ((float) y / cubert_NUMPART));
                    cl_float zf = 1.2 * cubert_NUMPART * particle_diameter
                                  * (-0.5 + rand_offset + ((float) z / cubert_NUMPART));
                    positions[pos_len] = (cl_float3) {xf, yf, zf};
                }
                pos_len++;
            }
        }
    }

    return (float) (1.2 * cubert_NUMPART * particle_diameter);
}