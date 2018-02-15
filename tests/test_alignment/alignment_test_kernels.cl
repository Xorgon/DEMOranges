__kernel void test_particle_struct_alignment(__global particle *particles, __global bool *correct) {
        int gid = get_global_id(0);

        if (!(particles[gid].pos.x == 20 &&
              particles[gid].pos.y == 21 &&
              particles[gid].pos.z == 22 &&
              particles[gid].vel.x == 23 &&
              particles[gid].vel.y == 24 &&
              particles[gid].vel.z == 25 &&
              particles[gid].forces.x == 26 &&
              particles[gid].forces.y == 27 &&
              particles[gid].forces.z == 28 &&
              particles[gid].id == 29 &&
              particles[gid].cv_array_idx == 30 &&
              particles[gid].diameter == 31 &&
              particles[gid].effect_diameter == 32 &&
              particles[gid].density == 33 &&
              particles[gid].fluid_viscosity == 34)) {
            *correct = false;
        }

        particles[gid].pos.x = 80;
        particles[gid].pos.y = 81;
        particles[gid].pos.z = 82;
        particles[gid].vel.x = 83;
        particles[gid].vel.y = 84;
        particles[gid].vel.z = 85;
        particles[gid].forces.x = 86;
        particles[gid].forces.y = 87;
        particles[gid].forces.z = 88;
        particles[gid].id = 89;
        particles[gid].cv_array_idx = 90;
        particles[gid].diameter = 91;
        particles[gid].effect_diameter = 92;
        particles[gid].density = 93;
        particles[gid].fluid_viscosity = 94;
}

__kernel void test_pp_collision_struct_alignment(__global pp_collision *pp_collisions, __global bool *correct) {
        int gid = get_global_id(0);

        if (!(pp_collisions[gid].p1_id == 20
            && pp_collisions[gid].p2_id == 21
            )) {
            *correct = false;
        }

        pp_collisions[gid].p1_id = 80;
        pp_collisions[gid].p2_id = 81;
}

__kernel void test_pw_collision_struct_alignment(__global pw_collision *pw_collisions, __global bool *correct) {
        int gid = get_global_id(0);

        if (!(pw_collisions[gid].p_id == 20
            && pw_collisions[gid].w_id == 21
            )) {
            *correct = false;
        }

        pw_collisions[gid].p_id = 80;
        pw_collisions[gid].w_id = 81;
}

__kernel void test_aa_wall_struct_alignment(__global aa_wall *aa_walls, __global bool *correct) {
    int gid = get_global_id(0);

    if (!(aa_walls[gid].normal.x == 20
          && aa_walls[gid].normal.y == 21
          && aa_walls[gid].normal.z == 22
          && aa_walls[gid].max.x == 23
          && aa_walls[gid].max.y == 24
          && aa_walls[gid].max.z == 25
          && aa_walls[gid].min.x == 26
          && aa_walls[gid].min.y == 27
          && aa_walls[gid].min.z == 28)) {
        *correct = false;
    }

    aa_walls[gid].normal.x = 80;
    aa_walls[gid].normal.y = 81;
    aa_walls[gid].normal.z = 82;
    aa_walls[gid].max.x = 83;
    aa_walls[gid].max.y = 84;
    aa_walls[gid].max.z = 85;
    aa_walls[gid].min.x = 86;
    aa_walls[gid].min.y = 87;
    aa_walls[gid].min.z = 88;
}