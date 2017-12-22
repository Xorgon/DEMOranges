float get_particle_overlap(particle p1, particle p2) {
    return p1.diameter / 2 + p2.diameter / 2 - distance(p1.pos, p2.pos);
}

float3 get_collision_normal(particle p1, particle p2) {
    return normalize(p2.pos - p1.pos);
}

float3 get_normal_velocity(particle p1, particle p2) {
    float3 normal = get_collision_normal(p1, p2);
    return dot(p2.vel - p1.vel, normal) * normal;
}

float3 calculate_collision_normal_force(pp_collision col, particle p1, particle p2) {
    float3 force = col.stiffness * get_particle_overlap(p1, p2) * get_collision_normal(p1, p2)
                    - col.damping_coefficient * get_normal_velocity(p1, p2);
    return force;
}


/* Kernel to calculate particle-particle collisions. */

__kernel void calculate_pp_collision(__global pp_collision *collisions, __global particle *particles, float delta_t) {
    int gid = get_global_id(0);
    ulong p1_id = collisions[gid].p1_id;
    ulong p2_id = collisions[gid].p2_id;
    float overlap = get_particle_overlap(particles[p1_id], particles[p2_id]);
    if (overlap > 0) {
        float3 normal_force = calculate_collision_normal_force(collisions[gid], particles[p1_id], particles[p2_id]);
        atomicAdd_float3(&particles[p1_id].forces, - normal_force);
        atomicAdd_float3(&particles[p2_id].forces, normal_force);
    }
}