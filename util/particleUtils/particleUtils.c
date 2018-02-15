//
// Created by Elijah on 18/12/2017.
//

#include "particleUtils.h"

void printParticle(particle *part) {
    particle p = *part;
    printf("%i\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", (int) p.id, p.diameter, p.density,
           p.fluid_viscosity, p.pos.x, p.pos.y, p.pos.z, p.vel.x, p.vel.y, p.vel.z, p.forces.x, p.forces.y, p.forces.z);
}

void writeParticles(particle *particles, float time, char prefix[], char dir[], cl_ulong NUMPART) {
    char filename[500];
    sprintf(filename, "%llu_%s%s_%i.txt", NUMPART, dir, prefix, (int) roundf(time * 1000));
    FILE *fd = fopen(filename, "w");
    for (int i = 0; i < NUMPART; i++) {
        fprintf(fd, "%f,%f,%f\n", particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
    }
    fclose(fd);
}

float get_particle_mass(particle *p) {
    return (float) ((*p).density * PI * powf((*p).diameter, 3) / 6);
}