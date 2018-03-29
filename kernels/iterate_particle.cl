#ifndef DEFAULT_PARTICLE_GRAVITY
float3 get_gravity(particle p, float delta_t) {
    return (float3) {0, -9.81, 0};
}
#endif

#ifndef DEFAULT_PARTICLE_FLUID_VEL
float3 get_vel_fluid(particle p, float delta_t) {
    return (float3) {0, 0, 0};
}
#endif

float get_tau(particle p) {
    return p.density * p.diameter * p.diameter / (18 * p.fluid_viscosity);
}

float get_mass(particle p) {
    return p.density * M_PI_F * pow(p.diameter, 3) / 6;
}

float3 get_accel(particle p, float delta_t) {
    float tau = get_tau(p);
    float3 non_drag_a = get_gravity(p, delta_t) + p.forces / get_mass(p);
    return (get_vel_fluid(p, delta_t) - p.vel + tau * non_drag_a) / (tau + delta_t);
}

float3 iterate_velocity(particle p, float delta_t) {
    float3 next_vel = p.vel + delta_t * get_accel(p, delta_t);
    return next_vel;
}

float3 iterate_position(particle p, float delta_t, float3 next_vel) {
    return p.pos + delta_t * (next_vel + p.vel) / 2;
}

/* Kernel to iterate particles. */
__kernel void iterate_particle(__global particle *particles, float delta_t, int int_periodic, float domain_length) {
    bool periodic = (bool) int_periodic;
    int gid = get_global_id(0);
    if (particles[gid].density == -1) {
        return; // -1 is used to denote infinite density.
    }
    float3 next_vel = iterate_velocity(particles[gid], delta_t);
    float3 next_pos = iterate_position(particles[gid], delta_t, next_vel);
    particles[gid].pos = next_pos;
    particles[gid].vel = next_vel;
    particles[gid].forces = (float3) {0, 0, 0};

    if (periodic) {
        if (particles[gid].pos.x > domain_length / 2) {
            particles[gid].pos.x -= domain_length;
        } else if (particles[gid].pos.x < - domain_length / 2) {
            particles[gid].pos.x += domain_length;
        }

        if (particles[gid].pos.y > domain_length / 2) {
            particles[gid].pos.y -= domain_length;
        } else if (particles[gid].pos.y < - domain_length / 2) {
            particles[gid].pos.y += domain_length;
        }

        if (particles[gid].pos.z > domain_length / 2) {
            particles[gid].pos.z -= domain_length;
        } else if (particles[gid].pos.z < - domain_length / 2) {
            particles[gid].pos.z += domain_length;
        }
    }
}