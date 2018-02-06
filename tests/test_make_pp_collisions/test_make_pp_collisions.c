//
// Created by Elijah on 06/02/2018.
//

#include "test_make_pp_collisions.h"

boolean test_count_pp_collisions(boolean verbose) {
    particle *hparticles;
    cl_mem gparticles;

    cl_int *particle_count_array; // Array of CVs with just the number of particles in each CV.
    cl_mem gparticle_count_array;

    cl_int cvs_per_edge = 3;
    cl_ulong NUMCVS = 27;

    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_int ret;

    printf("\nTesting pp_collision counting.\n");

    // Initializing OpenCL.
    setDevices(&platforms, &devices, FALSE);
    cl_context context = getContext(&devices, verbose);
    cl_command_queue queue = getCommandQueue(&context, &devices, verbose);
    cl_kernel count_pp_collisions = getKernel(&devices, &context, "../kernels/make_pp_collisions.cl",
                                              "count_pp_collisions", verbose);

    particle_count_array = calloc(27, sizeof(cl_int));
    particle_count_array[0] = 1; // CV 0, 0, 0
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

    return (collision_count == 12);
}

boolean test_make_pp_collisions(boolean verbose) {

}