//
// Created by Elijah on 18/12/2017.
//

#include "test_kernels/test_kernels.h"
#include "test_alignment/test_alignment.h"
#include "test_atomics/test_atomics.h"

#define VERBOSE FALSE

int main() {
    if (!test_kernels(VERBOSE)) {
        fprintf(stderr, "FAILED AT test_kernels");
        return 1;
    }

    if (!test_particle_struct_alignment(VERBOSE)) {
        fprintf(stderr, "FAILED AT test_particle_struct_alignment");
        return 1;
    }

    if (!test_pp_collision_struct_alignment(VERBOSE)) {
        fprintf(stderr, "FAILED AT test_pp_collision_struct_alignment");
        return 1;
    }

    if (!test_atomics(VERBOSE)) {
        fprintf(stderr, "FAILED AT test_atomics");
        return 1;
    }

    printf("All tests passed.");
}