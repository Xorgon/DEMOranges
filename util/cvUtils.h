//
// Created by Elijah on 29/01/2018.
//

#include <CL/cl_platform.h>
#include <math.h>

#ifndef DEMORANGES_DOMAINUTILS_H
#define DEMORANGES_DOMAINUTILS_H

void create_domain_count_cvs(cl_int **particle_count_array, cl_float *cv_length, cl_int *cvs_per_edge,
                             float domain_length, float particle_diameter);

cl_int get_array_idx(cl_int3 cv_coords, cl_int cvs_per_edge);

void set_array_starts(cl_int *particle_count_array, cl_ulong **cv_array_starts, cl_ulong NUMCVS);

#endif //DEMORANGES_DOMAINUTILS_H
