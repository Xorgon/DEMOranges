//
// Created by Elijah on 22/12/2017.
//

#include "collisionUtils.h"

float get_reduced_particle_mass(particle *p1, particle *p2) {
    float m1 = get_particle_mass(p1);
    float m2 = get_particle_mass(p2);
    return m1 * m2 / (m1 + m2);
}

float get_damping_coefficient(float restitution_coeff, float stiffness, float reduced_particle_mass) {
    float ln_rest = logf(restitution_coeff);
    return -2 * ln_rest * (float) pow(reduced_particle_mass * stiffness / (PI * PI + ln_rest * ln_rest), 0.5);
}
