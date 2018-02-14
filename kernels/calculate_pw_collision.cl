/* Kernel to calculate particle-wall collisions. */

float get_overlap(particle p, aa_wall wall) {
    return 0.5 * p.diameter  - fabs(dot(wall.max - p.pos, wall.normal));
}

float3 get_collision_normal(particle p, aa_wall wall) {
    return normalize(dot(p.pos - wall.max, wall.normal) * wall.normal);
}

float3 get_normal_velocity(particle p, float3 normal) {
    return dot(p.vel, normal) * normal;
}

float3 get_collision_tangent(particle p, float3 normal) {
    return normalize(p.vel - get_normal_velocity(p, normal));
}

float3 get_tangent_velocity(particle p, float3 tangent) {
    return dot(p.vel, tangent) * tangent;
}

bool is_in_wall_bounds(particle p, aa_wall wall, float3 normal) {
    float3 dif_max = wall.max - p.pos;
    float3 dif_min = p.pos - wall.min;

    float3 tang_dif_max = dif_max - dot(dif_max, normal) * normal;
    float3 tang_dif_min = dif_min - dot(dif_min, normal) * normal;

    return (tang_dif_max.x >= 0 && tang_dif_max.y >= 0 && tang_dif_max.z >= 0
            && tang_dif_min.x >= 0 && tang_dif_min.y >= 0 && tang_dif_min.z >= 0);
}


float3 calculate_normal_force(particle p, aa_wall wall, float overlap, float3 normal, float stiffness,
                                        float damping_coefficient) {
    return stiffness * overlap * normal - damping_coefficient * get_normal_velocity(p, normal);
}

float3 calculate_tangential_force(particle p, float friction_coefficient, float friction_stiffness,
                                  float3 normal_force, float3 normal, float delta_t) {
    float3 tangent = get_collision_tangent(p, normal);

    float3 f_dynamic = - friction_coefficient * length(normal_force) * tangent;

    float tang_displacement = length(get_tangent_velocity(p, tangent) * delta_t);
    float3 f_static = - friction_stiffness * tang_displacement * tangent;

    if (fast_length(f_dynamic) < fast_length(f_static)) {
        return f_dynamic;
    } else {
        return f_static;
    }
}

__kernel void calculate_pw_collision(__global pw_collision *collisions, __global particle *particles, float delta_t,
                                     __global aa_wall *walls, float stiffness, float restitution_coefficient,
                                     float friction_coefficient, float friction_stiffness) {
    int gid = get_global_id(0);
    ulong p_id = collisions[gid].p_id;
    ulong w_id = collisions[gid].w_id;

    float overlap = get_overlap(particles[p_id], walls[w_id]);
    float3 normal = get_collision_normal(particles[p_id], walls[w_id]);
    if (overlap > 0 && is_in_wall_bounds(particles[p_id], walls[w_id], normal)) {
        float reduced_particle_mass = get_particle_mass(particles[p_id]) / 2;
        float damping_coefficient = get_damping_coefficient(restitution_coefficient, stiffness, reduced_particle_mass);

        float3 normal_force =  calculate_normal_force(particles[p_id], walls[w_id], overlap, normal, stiffness,
                                                                damping_coefficient);
        float3 tangential_force = calculate_tangential_force(particles[p_id], friction_coefficient, friction_stiffness,
                                                                normal_force, normal, delta_t);
        atomicAdd_float3(&particles[p_id].forces, normal_force + tangential_force);
    }
}