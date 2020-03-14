//
// Created by Elijah on 18/12/2017.
//

#include "run_tests.h"


boolean run_all_tests(cl_device_id device, cl_context context, boolean verbose){
    printf("Running tests.\n");

    if (!test_kernels(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_kernels\n");
        return FALSE;
    }

    if (!test_particle_struct_alignment(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_particle_struct_alignment\n");
        return FALSE;
    }

    if (!test_pp_collision_struct_alignment(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_pp_collision_struct_alignment\n");
        return FALSE;
    }

    if (!test_pw_collision_struct_alignment(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_pw_collision_struct_alignment\n");
        return FALSE;
    }

    if (!test_aa_wall_struct_alignment(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_aa_wall_struct_alignment\n");
        return FALSE;
    }

    if (!test_atomics(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_atomics\n");
        return FALSE;
    }

    if (!test_reset_particle_count(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_reset_particle_count\n");
        return FALSE;
    }
    
    if (!test_assign_particle_count(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_assign_particle_count\n");
        return FALSE;
    }

    if (!test_set_array_starts(verbose)) {
        fprintf(stderr, "\nFAILED AT test_set_array_starts\n");
        return FALSE;
    }

    if (!test_assign_particles(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_assign_particles\n");
        return FALSE;
    }

    if (!test_count_pp_collisions(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_count_pp_collisions\n");
        return FALSE;
    }

    if (!test_make_pp_collisions(device, context, verbose)) {
        fprintf(stderr, "\nFAILED AT test_make_pp_collisions\n");
        return FALSE;
    }

    printf("All tests passed.\n");

    return TRUE;
}

