/* Kernel to make collision list for particle-particle collisions. */

/*
For all CVs, go through all the particles and all the neighbours.
Only create a collision if the particle being collided with others has a pid less than the other.
This is essentially what a naive approach does but for a limited number of particles.
*/

__kernel void count_pp_collisions(__global int *particle_count_array, int cvs_per_edge,
                                    __global ulong *collision_count, int int_periodic){
    bool periodic;
    if (int_periodic == 0) {
        periodic = false;
    } else {
        periodic = true;
    }
    ulong cv_idx = get_global_id(0);
    ulong count_to_add = 0;
    int3 coords = cv_array_idx_to_cv_coords(cv_idx, cvs_per_edge);
    for (int x = coords.x - 1; x <= coords.x + 1; x++) {
        if (x >= cvs_per_edge || x < 0) {
            continue; // Skip if out of bounds in the x direction.
        }
        for (int y = coords.y - 1; y <= coords.y + 1; y++) {
            if (y >= cvs_per_edge || y < 0) {
                continue; // Skip if out of bounds in the y direction.
            }
            for (int z = coords.z - 1; z <= coords.z + 1; z++) {
                if (z >= cvs_per_edge || z < 0) {
                    continue; // Skip if out of bounds in the z direction.
                }
                int3 other_cv_coords = (int3) {x, y, z};
                ulong other_cv_idx = cv_coords_to_cv_array_idx(other_cv_coords, cvs_per_edge);
                if (other_cv_idx == cv_idx) {
                    // Don't do collisions for periodic edge CVs.
                    if (!periodic && !(cv_idx == 0 || cv_idx == cvs_per_edge - 1)) {
                        count_to_add += particle_count_array[cv_idx] * (particle_count_array[cv_idx] - 1) / 2;
                    }
                } else if (other_cv_idx > cv_idx) { // To avoid counting the same collisions twice.
                    count_to_add += particle_count_array[cv_idx] * particle_count_array[other_cv_idx];
                }
            }
        }
    }
    atom_add(collision_count, count_to_add);
}

__kernel void make_pp_collisions(__global ulong *cv_start_array, __global int *particle_count_array,
                                    __global ulong *cv_pids, int cvs_per_edge, __global pp_collision *collisions,
                                    __global ulong *collision_count, int int_periodic){
    bool periodic;
    if (int_periodic == 0) {
        periodic = false;
    } else {
        periodic = true;
    }
    ulong cv_idx = get_global_id(0);
    int3 coords = cv_array_idx_to_cv_coords(cv_idx, cvs_per_edge);
    for (int i = 0; i < particle_count_array[cv_idx]; i++) {
        for (int x = coords.x - 1; x <= coords.x + 1; x++) {
            if (x >= cvs_per_edge || x < 0) {
                continue; // Skip if out of bounds in the x direction.
            }
            for (int y = coords.y - 1; y <= coords.y + 1; y++) {
                if (y >= cvs_per_edge || y < 0) {
                    continue; // Skip if out of bounds in the y direction.
                }
                for (int z = coords.z - 1; z <= coords.z + 1; z++) {
                    if (z >= cvs_per_edge || z < 0) {
                        continue; // Skip if out of bounds in the z direction.
                    }
                    int3 other_cv_coords = (int3) {x, y, z};
                    ulong other_cv_idx = cv_coords_to_cv_array_idx(other_cv_coords, cvs_per_edge);
                    if (other_cv_idx == cv_idx && periodic && (cv_idx == 0 || cv_idx == cvs_per_edge - 1)) {
                        continue; // Don't do collisions for periodic edge CVs.
                    }
                    for (int j = 0; j < particle_count_array[other_cv_idx]; j++){
                        ulong this_pid = cv_pids[cv_start_array[cv_idx] + i];
                        ulong other_pid = cv_pids[cv_start_array[other_cv_idx] + j];
                        if (this_pid >= other_pid) {
                            continue; // Skip if it's the same particle or a particle with a lower ID.
                        }
                        ulong col_idx = atom_inc(collision_count);
                        collisions[col_idx].p1_id = this_pid;
                        collisions[col_idx].p2_id = other_pid;
                    }
                }
            }
        }
    }
}
