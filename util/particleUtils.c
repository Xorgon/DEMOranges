//
// Created by Elijah on 18/12/2017.
//

#include "particleUtils.h"

void printParticle(particle *part) {
    particle p = *part;
    printf("%i\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", (int) p.particle_id, p.particle_diameter, p.density,
           p.fluid_viscosity, p.pos.x, p.pos.y, p.pos.z, p.vel.x, p.vel.y, p.vel.z, p.forces.x, p.forces.y, p.forces.z);
}