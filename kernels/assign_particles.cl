/* Kernel to count the number of particles per control volume. */

__kernel void assign_particle_count(__global particle *particles, __global int *particle_count_array,
    float domain_length, float cv_length, int cvs_per_edge){
    int gid = get_global_id(0);
    int3 cv_coords = pos_to_cv_coords(particles[gid].pos, domain_length, cv_length, cvs_per_edge);
    if (cv_coords.x == -1 && cv_coords.y == -1 && cv_coords.z == -1) {
	particles[gid].cv_array_idx = -1;
        return;
    }
    int cv_array_idx = cv_coords_to_cv_array_idx(cv_coords, cvs_per_edge);
    if (cv_array_idx > cvs_per_edge * cvs_per_edge * cvs_per_edge) {
        printf("Warning: assign particle count failed at, pos: (%.8f, %.8f, %.8f), cv: (%i, %i, %i)\n", particles[gid].pos.x, particles[gid].pos.y, particles[gid].pos.z, cv_coords.x, cv_coords.y, cv_coords.z);
    }
    particles[gid].cv_array_idx = cv_array_idx;
    atomic_inc(&(particle_count_array[cv_array_idx]));
}


/* Kernel to assign particles to control volumes. */

__kernel void assign_particles(__global particle *particles, __global ulong *cv_start_array,
                               __global int *input_count_array, __global ulong *cv_pids) {
    int gid = get_global_id(0); // Equivalent to particle ID.
    if (particles[gid].cv_array_idx == -1) {
        return;
    }
    int cv_array_idx = particles[gid].cv_array_idx;
    int cv_pid_mini_idx = atomic_inc(&(input_count_array[cv_array_idx])); // Index within the CV.
    if (cv_start_array[cv_array_idx] == (ulong) -1) {
        printf("WARNING: Particle assigned to array with particle count of 0. idx = %i, pos: (%.8f, %.8f, %.8f), max_idx = %i\n", cv_array_idx, particles[gid].pos.x, particles[gid].pos.y, particles[gid].pos.z, get_global_size(0));
        return;
    }
    int cv_pid_idx = cv_start_array[cv_array_idx] + cv_pid_mini_idx;
    cv_pids[cv_pid_idx] = gid;
}
