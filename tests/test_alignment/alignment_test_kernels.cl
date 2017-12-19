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
              particles[gid].particle_id == 29 &&
              particles[gid].particle_diameter == 30 &&
              particles[gid].density == 31 &&
              particles[gid].fluid_viscosity == 32)) {
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
        particles[gid].particle_id = 89;
        particles[gid].particle_diameter = 90;
        particles[gid].density = 91;
        particles[gid].fluid_viscosity = 92;
}
