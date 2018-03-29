/* Kernel to make collision list for particle-particle collisions. */

/*
For all CVs, go through all the particles and all the neighbours.
Only create a collision if the particle being collided with others has a pid less than the other.
This is essentially what a naive approach does but for a limited number of particles.
*/

__kernel void count_pp_collisions(__global int *particle_count_array, int cvs_per_edge,
                                    __global ulong *collision_count, int int_periodic){
    bool periodic = (bool) int_periodic;
    ulong cv_idx = get_global_id(0);
    ulong count_to_add = 0;
    int3 coords = cv_array_idx_to_cv_coords(cv_idx, cvs_per_edge);
    for (int x = coords.x - 1; x <= coords.x + 1; x++) {
        if ((x >= cvs_per_edge || x < 0) && !periodic) {
            continue;
        }
        int other_x;
        if (x >= cvs_per_edge) {
            other_x = 0;
        } else if (x < 0) {
            other_x = cvs_per_edge - 1;
        } else {
            other_x = x;
        }
        for (int y = coords.y - 1; y <= coords.y + 1; y++) {
            if ((y >= cvs_per_edge || y < 0) && !periodic) {
                continue;
            }
            int other_y;
            if (y >= cvs_per_edge) {
                other_y = 0;
            } else if (y < 0) {
                other_y = cvs_per_edge - 1;
            } else {
                other_y = y;
            }
            for (int z = coords.z - 1; z <= coords.z + 1; z++) {
                if ((z >= cvs_per_edge || z < 0) && !periodic) {
                    continue;
                }
                int other_z;
                if (z >= cvs_per_edge) {
                    other_z = 0;
                } else if (z < 0) {
                    other_z = cvs_per_edge - 1;
                } else {
                    other_z = z;
                }

                // Actual counting code.
                int3 other_cv_coords = (int3) {other_x, other_y, other_z};
                ulong other_cv_idx = cv_coords_to_cv_array_idx(other_cv_coords, cvs_per_edge);
                if (other_cv_idx == cv_idx) {
                    count_to_add += particle_count_array[cv_idx] * (particle_count_array[cv_idx] - 1) / 2;
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
    bool periodic = (bool) int_periodic;
    ulong cv_idx = get_global_id(0);
    int3 coords = cv_array_idx_to_cv_coords(cv_idx, cvs_per_edge);
    for (int i = 0; i < particle_count_array[cv_idx]; i++) {
        for (int x = coords.x - 1; x <= coords.x + 1; x++) {
            if ((x >= cvs_per_edge || x < 0) && !periodic) {
                continue;
            }
            char cross_boundary_x = 0;
            int other_x;
            if (x >= cvs_per_edge) {
                other_x = 0;
                cross_boundary_x = 1;
            } else if (x < 0) {
                other_x = cvs_per_edge - 1;
                cross_boundary_x = -1;
            } else {
                other_x = x;
            }
            for (int y = coords.y - 1; y <= coords.y + 1; y++) {
                if ((y >= cvs_per_edge || y < 0) && !periodic) {
                    continue;
                }
                char cross_boundary_y = 0;
                int other_y;
                if (y >= cvs_per_edge) {
                    other_y = 0;
                    cross_boundary_y = 1;
                } else if (y < 0) {
                    other_y = cvs_per_edge - 1;
                    cross_boundary_y = -1;
                } else {
                    other_y = y;
                }
                for (int z = coords.z - 1; z <= coords.z + 1; z++) {
                    if ((z >= cvs_per_edge || z < 0) && !periodic) {
                        continue;
                    }
                    char cross_boundary_z = 0;
                    int other_z;
                    if (z >= cvs_per_edge) {
                        other_z = 0;
                        cross_boundary_z = 1;
                    } else if (z < 0) {
                        other_z = cvs_per_edge - 1;
                        cross_boundary_z = -1;
                    } else {
                        other_z = z;
                    }

                    int3 other_cv_coords = (int3) {other_x, other_y, other_z};
                    ulong other_cv_idx = cv_coords_to_cv_array_idx(other_cv_coords, cvs_per_edge);
                    for (int j = 0; j < particle_count_array[other_cv_idx]; j++){
                        ulong this_pid = cv_pids[cv_start_array[cv_idx] + i];
                        ulong other_pid = cv_pids[cv_start_array[other_cv_idx] + j];
                        if (this_pid >= other_pid) {
                            continue; // Skip if it's the same particle or a particle with a lower ID.
                        }
                        ulong col_idx = atom_inc(collision_count);
                        collisions[col_idx].p1_id = this_pid;
                        collisions[col_idx].p2_id = other_pid;
                        collisions[col_idx].cross_boundary_x = cross_boundary_x;
                        collisions[col_idx].cross_boundary_y = cross_boundary_y;
                        collisions[col_idx].cross_boundary_z = cross_boundary_z;
                    }
                }
            }
        }
    }
}
