//
// Created by Elijah on 19/12/2017.
//
#include "test_kernels.h"

boolean test_kernels(boolean verbose) {
    cl_platform_id *platforms;
    cl_device_id *devices;

    cl_int ret;

    // Source file variables
    char *fileNames[] = {
            "../kernels/iterate_particle.cl",
//        "../kernels/make_pp_collisions.cl",
//        "../kernels/make_pw_collisions.cl",
            "../kernels/calculate_pp_collision.cl",
            "../kernels/calculate_pw_collision.cl",
//        "../kernels/sort_particles.cl"
    };

    char *kernelNames[] = {
            "iterate_particle",
//        "make_pp_collisions",
//        "make_pw_collisions",
            "calculate_pp_collision",
            "calculate_pw_collision",
//        "sort_particles"
    };
    int files = 3;

    setDevices(&platforms, &devices, FALSE);

    // Create OpenCL context
    cl_context context = NULL;
    context = clCreateContext(NULL, 1, &devices[0], NULL, NULL, &ret);
    if (verbose) printf("[INIT] Create OpenCL context: ");

    if ((int) ret == 0) {
        if (verbose) printf("SUCCESS\n");
    } else {
        if (verbose) printf("FAILED\n");
        _getch();
        return FALSE;
    }

    for (int i = 0; i < files; i++) {

        if (verbose) {
            printf("\nKernel file: %s\n", fileNames[i]);
            printf("Kernel name: %s\n", kernelNames[i]);
        }

        cl_kernel kernel = getKernel(&devices, &context, fileNames[i], kernelNames[i], verbose);
        if (kernel == NULL) {
            return FALSE;
        }
    }
    return TRUE;
}
