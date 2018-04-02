//
// Created by Elijah on 29/01/2018.
//

#include "cvUtils.h"

void create_domain_count_cvs(cl_int **particle_count_array, cl_float *cv_length, cl_int *cvs_per_edge,
                             float domain_length, float particle_diameter) {
    *cvs_per_edge = (cl_int) floorf(domain_length / particle_diameter);
    *cv_length = domain_length / *cvs_per_edge;
    int total_cvs = (int) pow(*cvs_per_edge, 3);
    *particle_count_array = calloc(total_cvs, sizeof(cl_int));
}

cl_int get_array_idx(cl_int3 cv_coords, cl_int cvs_per_edge) {
    return cv_coords.x + cv_coords.y * cvs_per_edge + cv_coords.z * cvs_per_edge * cvs_per_edge;
}

void set_array_starts(cl_int *particle_count_array, cl_ulong *cv_array_starts, cl_ulong NUMCVS) {
    cl_ulong idx = 0;
    for (cl_ulong i = 0; i < NUMCVS; i++) {
        cl_int count = particle_count_array[i];
        if (count > 0) {
            cv_array_starts[i] = idx;
            idx += count;
        } else {
            cv_array_starts[i] = (cl_ulong) -1;
        }
    }
}