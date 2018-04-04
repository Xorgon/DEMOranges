//
// Created by Elijah on 18/12/2017.
//

#include "particleUtils.h"

void printParticle(particle *part) {
    particle p = *part;
    printf("%i\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", (int) p.id, p.diameter, p.density,
           p.fluid_viscosity, p.pos.x, p.pos.y, p.pos.z, p.vel.x, p.vel.y, p.vel.z, p.forces.x, p.forces.y, p.forces.z);
}

float get_particle_mass_from_values(float density, float diameter) {
    return (float) density * PI * powf(diameter, 3) / 6);
}

float get_particle_mass(particle *p) {
    return (float) ((*p).density * PI * powf((*p).diameter, 3) / 6);
}

float get_tau(particle p) {
    return p.density * p.diameter * p.diameter / (18 * p.fluid_viscosity);
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

float createCubePositions(cl_float3 *positions, cl_ulong NUMPART, float particle_diameter, float spacing_factor) {
    cl_ulong cubert_NUMPART = (cl_ulong) ceil(pow(NUMPART, 0.334));
    cl_ulong pos_len = 0;
    srand(0);

    float cube_length = spacing_factor * cubert_NUMPART * particle_diameter;
    for (int x = 0; x < cubert_NUMPART; x++) {
        for (int y = 0; y < cubert_NUMPART; y++) {
            for (int z = 0; z < cubert_NUMPART; z++) {
                if (pos_len < NUMPART) {
                    float rand_offset = 0.005 * (float) rand() / (float) (RAND_MAX);
                    cl_float xf = cube_length
                                  * (-0.5 + rand_offset + ((float) x / cubert_NUMPART));
                    cl_float yf = cube_length
                                  * (-0.5 + rand_offset + ((float) y / cubert_NUMPART));
                    cl_float zf = cube_length
                                  * (-0.5 + rand_offset + ((float) z / cubert_NUMPART));
                    positions[pos_len] = (cl_float3) {xf, yf, zf};
                }
                pos_len++;
            }
        }
    }

    return (float) cube_length;
}

void createNormalDistVelocities(cl_float3 *velocities, cl_ulong NUMPART, float mean, float std_dev) {
    srand(0);

    for (int i = 0; i < NUMPART; i++) {
        // Select random speed.
        // Box-Muller transformation from uniform to normal.
        float u = (float) rand() / (float) (RAND_MAX);
        float v =(float) rand() / (float) (RAND_MAX);
        float speed = (float) (std_dev * sqrtf(-2 * logf(u)) * cos(2 * PI * v)) + mean;

        // Select random direction.
        float pitch = (float) rand() / (float) (RAND_MAX) * PI; // From -ve z (0) to +ve z (pi).
        float yaw = (float) rand() / (float) (RAND_MAX) * 2 * PI; // +ve x = 0, -ve y = pi/2, -ve x = pi, +ve y = 3pi/4.

        float x = speed * cosf(pitch) * cosf(yaw);
        float y = speed * cosf(pitch) * sinf(yaw);
        float z = speed * sinf(pitch);

        velocities[i] = (cl_float3) {x, y, z};
    }
}