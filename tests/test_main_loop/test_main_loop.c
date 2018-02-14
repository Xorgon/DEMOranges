//
// Created by Elijah on 08/02/2018.
//

#include "test_main_loop.h"

boolean test_main_loop(cl_device_id device, cl_context context, boolean verbose) {
    particle *hparticles;
    cl_mem gparticles;

    cl_int *particle_count_array; // Array of CVs with just the number of particles in each CV.
    cl_mem gparticle_count_array;

    cl_float cv_length;
    cl_int cvs_per_edge;
    cl_float domain_length = 3;
    cl_ulong NUMCVS; // Total number of CVs.

    cl_ulong *cv_start_array;
    cl_mem gcv_start_array;

    cl_int *input_count_array;
    cl_mem ginput_count_array;

    cl_ulong *cv_pids;
    cl_mem gcv_pids;

    pp_collision *collisions;
    cl_mem gcollisions;

    cl_ulong NUMPART = 6;

    cl_int ret;

    if (verbose) printf("\nTesting main loop functionality.\n");

    // Initializing OpenCL.
    cl_command_queue queue = getCommandQueue(context, device, verbose);
    cl_kernel assign_particle_count = getKernel(device, context, "../kernels/assign_particles.cl",
                                                "assign_particle_count", verbose);
    cl_kernel assign_particles = getKernel(device, context, "../kernels/assign_particles.cl",
                                           "assign_particles", verbose);
    cl_kernel count_pp_collisions = getKernel(device, context, "../kernels/make_pp_collisions.cl",
                                              "count_pp_collisions", verbose);
    cl_kernel make_pp_collisions = getKernel(device, context, "../kernels/make_pp_collisions.cl",
                                             "make_pp_collisions", verbose);

    hparticles = malloc(sizeof(particle) * NUMPART);

    cl_float particle_diameter = 1;

    for (cl_ulong i = 0; i < NUMPART; i++) {
        hparticles[i].id = i;
        hparticles[i].density = 2000;
        hparticles[i].fluid_viscosity = 0.0000193 * 100;
        hparticles[i].diameter = particle_diameter;
        hparticles[i].vel = (cl_float3) {0.0, 0.0, 0.0};
        hparticles[i].forces = (cl_float3) {0.0, 0.0, 0.0};
    }

    // Note scrambled particle ID order to ensure it doesn't accidentally end up the same.
    hparticles[4].pos = (cl_float3) {-1.4, -1.4, -1.4}; // 'minimum' CV.
    hparticles[5].pos = (cl_float3) {0, 0, 0}; // 'middle' CV.
    hparticles[1].pos = (cl_float3) {0, 0, 0}; // 'middle' CV.
    hparticles[3].pos = (cl_float3) {1.4, 1.4, 1.4}; // 'maximum' CV.
    hparticles[0].pos = (cl_float3) {1.4, 1.4, 1.4}; // 'maximum' CV.
    hparticles[2].pos = (cl_float3) {1.4, 1.4, 1.4}; // 'maximum' CV.

    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);
    ret = particlesToDevice(queue, gparticles, &hparticles, NUMPART);

    create_domain_count_cvs(&particle_count_array, &cv_length, &cvs_per_edge, domain_length, particle_diameter);
    NUMCVS = (cl_ulong) cvs_per_edge * cvs_per_edge * cvs_per_edge;
    gparticle_count_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * NUMCVS, NULL, &ret);
    ret = intArrayToDevice(queue, gparticle_count_array, &particle_count_array, NUMCVS);
    cv_start_array = malloc(sizeof(cl_ulong) * NUMCVS);

    ret = clSetKernelArg(assign_particle_count, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(assign_particle_count, 1, sizeof(cl_mem), &gparticle_count_array);
    ret = clSetKernelArg(assign_particle_count, 2, sizeof(cl_float), &domain_length);
    ret = clSetKernelArg(assign_particle_count, 3, sizeof(cl_float), &cv_length);
    ret = clSetKernelArg(assign_particle_count, 4, sizeof(cl_int), &cvs_per_edge);
    ret = clEnqueueNDRangeKernel(queue, assign_particle_count, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);
    ret = clFinish(queue);
    ret = intArrayToHost(queue, gparticle_count_array, &particle_count_array, NUMCVS);

    set_array_starts(particle_count_array, cv_start_array, NUMCVS);
    gcv_start_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * NUMCVS, NULL, &ret);
    ret = ulongArrayToDevice(queue, gcv_start_array, &cv_start_array, NUMCVS);

    input_count_array = calloc(NUMCVS, sizeof(cl_int));
    ginput_count_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * NUMCVS, NULL, &ret);
    ret = intArrayToDevice(queue, ginput_count_array, &input_count_array, NUMCVS);

    cv_pids = malloc(sizeof(cl_ulong) * NUMPART);
    gcv_pids = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * NUMPART, NULL, &ret);

    ret = clSetKernelArg(assign_particles, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(assign_particles, 1, sizeof(cl_mem), &gcv_start_array);
    ret = clSetKernelArg(assign_particles, 2, sizeof(cl_mem), &ginput_count_array);
    ret = clSetKernelArg(assign_particles, 3, sizeof(cl_mem), &gcv_pids);

    ret = clEnqueueNDRangeKernel(queue, assign_particles, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);
    clFinish(queue);

    ulongArrayToHost(queue, gcv_pids, &cv_pids, NUMPART);
//    for (int i = 0; i < NUMPART; i++) {
//        printf("%llu\n", cv_pids[i]);
//    }

    cl_ulong collision_count = 0;
    cl_mem gcollision_count = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong), NULL, &ret);
    ret = clEnqueueWriteBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);

    clSetKernelArg(count_pp_collisions, 0, sizeof(cl_mem), &gparticle_count_array);
    clSetKernelArg(count_pp_collisions, 1, sizeof(cl_int), &cvs_per_edge);
    clSetKernelArg(count_pp_collisions, 2, sizeof(cl_mem), &gcollision_count);

    ret = clEnqueueNDRangeKernel(queue, count_pp_collisions, 1, NULL, &NUMCVS, 0, NULL, NULL, NULL);
    clFinish(queue);

    clEnqueueReadBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);

    cl_ulong NUMCOLS = collision_count;

    collisions = malloc(sizeof(pp_collision) * NUMCOLS);
    collision_count = 0;
    gcollisions = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(pp_collision) * NUMCOLS, NULL, &ret);
    ulongArrayToDevice(queue, gcv_pids, &cv_pids, NUMPART);

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

    if (verbose) printf("Collision count = %llu\n", collision_count);

    return (collision_count == NUMCOLS);
}