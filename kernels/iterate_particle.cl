/* Kernel to iterate particles. */
__kernel void iterate_particle(__global particle *particles) {
        int gid = get_global_id(0);
        particles[gid].pos.y *= particles[gid].pos.y;
//        particles[gid].particle_id = 90;
//        particles[gid].particle_diameter = 91;
//        particles[gid].density = 92;
//        particles[gid].fluid_viscosity = 93;
//        particles[gid].pos.x = 94;
//        particles[gid].pos.y = 95;
//        particles[gid].pos.z = 96;
//        particles[gid].vel.x = 97;
//        particles[gid].vel.y = 98;
//        particles[gid].vel.z = 99;
//        particles[gid].forces.x = 100;
//        particles[gid].forces.y = 101;
//        particles[gid].forces.z = 102;
}