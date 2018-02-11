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
              particles[gid].density == 32 &&
              particles[gid].fluid_viscosity == 33)) {
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
        particles[gid].density = 92;
        particles[gid].fluid_viscosity = 93;
}

__kernel void test_pp_collision_struct_alignment(__global pp_collision *pp_collisions, __global bool *correct) {
        int gid = get_global_id(0);

        if (!(pp_collisions[gid].p1_id == 20
            && pp_collisions[gid].p2_id == 21
//            && pp_collisions[gid].stiffness == 22
//            && pp_collisions[gid].damping_coefficient == 23
//            && pp_collisions[gid].friction_coefficient == 24
//            && pp_collisions[gid].friction_stiffness == 25
            )) {
//            *correct = false;
        }

        pp_collisions[gid].p1_id = 80;
        pp_collisions[gid].p2_id = 81;
//        pp_collisions[gid].stiffness = 82;
//        pp_collisions[gid].damping_coefficient = 83;
//        pp_collisions[gid].friction_coefficient = 84;
//        pp_collisions[gid].friction_stiffness = 85;
}
