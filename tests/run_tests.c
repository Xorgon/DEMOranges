//
// Created by Elijah on 18/12/2017.
//

#include "test_kernels/test_kernels.h"
#include "test_alignment/test_alignment.h"
#include "test_atomics/test_atomics.h"
#include "test_assign_particles/test_assign_particles.h"
#include "test_make_pp_collisions/test_make_pp_collisions.h"
#include "test_main_loop/test_main_loop.h"
#include "test_setContext/test_setContext.h"

#define VERBOSE TRUE

int main() {

    cl_device_id device;
    cl_context context;

    setContext(&device, &context, TRUE);

    if (!test_kernels(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_kernels\n");
        return 1;
    }

    if (!test_particle_struct_alignment(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_particle_struct_alignment\n");
        return 1;
    }

    if (!test_pp_collision_struct_alignment(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_pp_collision_struct_alignment\n");
        return 1;
    }

    if (!test_atomics(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_atomics\n");
        return 1;
    }

    if (!test_assign_particle_count(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_assign_particle_count\n");
        return 1;
    }

    if (!test_set_array_starts(VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_set_array_starts\n");
        return 1;
    }

    if (!test_assign_particles(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_assign_particles\n");
        return 1;
    }

    if (!test_count_pp_collisions(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_count_pp_collisions\n");
        return 1;
    }

    if (!test_make_pp_collisions(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_make_pp_collisions\n");
        return 1;
    }
    
    if (!test_main_loop(device, context, VERBOSE)) {
        fprintf(stderr, "\nFAILED AT test_main_loop\n");
        return 1;
    }

//    if (!test_setContext(VERBOSE)) {
//        fprintf(stderr, "\nFAILED AT test_setContext\n");
//        return 1;
//    }
    
    printf("\nAll tests passed.\n");
}