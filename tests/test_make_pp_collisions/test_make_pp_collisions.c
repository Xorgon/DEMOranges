//
// Created by Elijah on 06/02/2018.
//

#include "test_make_pp_collisions.h"

boolean test_count_pp_collisions(boolean verbose) {
    printf("\nTesting pp_collision counting.\n");

    particle *hparticles;
    cl_mem gparticles;

    cl_int *particle_count_array; // Array of CVs with just the number of particles in each CV.
    cl_mem gparticle_count_array;

    cl_int cvs_per_edge = 3;
    cl_ulong NUMCVS = 27;

    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_int ret;


    // Initializing OpenCL.
    setDevices(&platforms, &devices, FALSE);
    cl_context context = getContext(&devices, verbose);
    cl_command_queue queue = getCommandQueue(&context, &devices, verbose);
    cl_kernel count_pp_collisions = getKernel(&devices, &context, "../kernels/make_pp_collisions.cl",
                                              "count_pp_collisions", verbose);

    particle_count_array = calloc(27, sizeof(cl_int));
    particle_count_array[0] = 1; // CV 0, 0, 0
    particle_count_array[1] = 1; // CV 1, 0, 0
    particle_count_array[9] = 1; // CV 0, 0, 1
    particle_count_array[13] = 2; // CV 1, 1, 1
    particle_count_array[26] = 3; // CV 2, 2, 2

    gparticle_count_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * NUMCVS, NULL, &ret);
    ret = intArrayToDevice(queue, gparticle_count_array, &particle_count_array, NUMCVS);

    cl_ulong collision_count = 0;
    cl_mem gcollision_count = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong), NULL, &ret);
    ret = clEnqueueWriteBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);
    gcollision_count = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong), NULL, &ret);

    clSetKernelArg(count_pp_collisions, 0, sizeof(cl_mem), &gparticle_count_array);
    clSetKernelArg(count_pp_collisions, 1, sizeof(cl_int), &cvs_per_edge);
    clSetKernelArg(count_pp_collisions, 2, sizeof(cl_mem), &gcollision_count);

    ret = clEnqueueNDRangeKernel(queue, count_pp_collisions, 1, NULL, &NUMCVS, 0, NULL, NULL, NULL);
    clFinish(queue);

    clEnqueueReadBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);

    return (collision_count == 19);
}

boolean test_make_pp_collisions(boolean verbose) {
    cl_ulong *cv_start_array;
    cl_mem gcv_start_array;

    cl_int *particle_count_array;
    cl_mem gparticle_count_array;

    cl_ulong *cv_pids;
    cl_mem gcv_pids;

    pp_collision *collisions;
    cl_mem gcollisions;

    cl_ulong collision_count;
    cl_mem gcollision_count;

    cl_int cvs_per_edge = 3;

    cl_ulong NUMCVS = 27;

    particle_count_array = calloc(27, sizeof(cl_int));
    particle_count_array[0] = 1; // CV 0, 0, 0
    particle_count_array[1] = 1; // CV 1, 0, 0
    particle_count_array[9] = 1; // CV 0, 0, 1
    particle_count_array[13] = 2; // CV 1, 1, 1
    particle_count_array[26] = 3; // CV 2, 2, 2

    cv_start_array = malloc(sizeof(cl_ulong) * NUMCVS);
    memset(cv_start_array, -1, NUMCVS);
    cv_start_array[0] = 0;
    cv_start_array[1] = 1;
    cv_start_array[9] = 2;
    cv_start_array[13] = 3;
    cv_start_array[26] = 5;


    cl_ulong NUMPARTS = 8;
    cv_pids = malloc(sizeof(cl_ulong) * NUMPARTS);
    for (cl_ulong i = 0; i < NUMPARTS; i++) {
        cv_pids[i] = i;
    }

    cl_ulong NUMCOLS = 19;
    collisions = malloc(sizeof(pp_collision) * NUMCOLS);
    collision_count = 0;

    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_int ret;

    printf("\nTesting pp_collision creation.\n");

    // Initializing OpenCL.
    setDevices(&platforms, &devices, FALSE);
    cl_context context = getContext(&devices, verbose);
    cl_command_queue queue = getCommandQueue(&context, &devices, verbose);
    cl_kernel make_pp_collisions = getKernel(&devices, &context, "../kernels/make_pp_collisions.cl",
                                             "make_pp_collisions", verbose);


    gcv_start_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * NUMCVS, NULL, &ret);
    gparticle_count_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * NUMCVS, NULL, &ret);
    gcv_pids = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * NUMPARTS, NULL, &ret);
    gcollisions = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(pp_collision) * NUMCOLS, NULL, &ret);
    gcollision_count = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong), NULL, &ret);

    ulongArrayToDevice(queue, gcv_start_array, &cv_start_array, NUMCVS);
    intArrayToDevice(queue, gparticle_count_array, &particle_count_array, NUMCVS);
    ulongArrayToDevice(queue, gcv_pids, &cv_pids, NUMPARTS);
    pp_collisionsToDevice(queue, gcollisions, &collisions, NUMCOLS);
    clEnqueueWriteBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);

    clSetKernelArg(make_pp_collisions, 0, sizeof(cl_mem), &gcv_start_array);
    clSetKernelArg(make_pp_collisions, 1, sizeof(cl_mem), &gparticle_count_array);
    clSetKernelArg(make_pp_collisions, 2, sizeof(cl_mem), &gcv_pids);
    clSetKernelArg(make_pp_collisions, 3, sizeof(cl_int), &cvs_per_edge);
    clSetKernelArg(make_pp_collisions, 4, sizeof(cl_mem), &gcollisions);
    clSetKernelArg(make_pp_collisions, 5, sizeof(cl_mem), &gcollision_count);

    clEnqueueNDRangeKernel(queue, make_pp_collisions, 1, NULL, &NUMCVS, 0, NULL, NULL, NULL);

    clEnqueueReadBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);
    pp_collisionsToHost(queue, gcollisions, &collisions, NUMCOLS);

    if (verbose) {
        printf("Collision count = %lu\n", (unsigned long) collision_count);

//        for (int i = 0; i < NUMCOLS; i++) {
//            printf("%llu -> %llu\n", collisions[i].p1_id, collisions[i].p2_id);
//        }
    }

    return (collision_count == NUMCOLS);
}