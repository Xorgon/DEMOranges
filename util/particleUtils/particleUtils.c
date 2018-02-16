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