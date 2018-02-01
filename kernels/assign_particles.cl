/* Kernel to count the number of particles per control volume. */

__kernel void assign_particle_count(__global particle *particles, __global int *particle_count_array,
    float domain_length, float cv_length, int cvs_per_edge){
    int gid = get_global_id(0);
    int cv_array_idx = cv_coords_to_cv_array_idx(pos_to_cv_coords(particles[gid].pos, domain_length, cv_length),
                                                cvs_per_edge);
    atomic_inc(&(particle_count_array[cv_array_idx]));
}


/* Kernel to assign particles to control volumes. */

__kernel void assign_particles() {}