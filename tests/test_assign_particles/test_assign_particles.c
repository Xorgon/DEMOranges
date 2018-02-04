//
// Created by Elijah on 31/01/2018.
//

#include "test_assign_particles.h"
#include "../../util/cvUtils.h"

boolean test_assign_particle_count(boolean verbose) {

    particle *hparticles;
    cl_mem gparticles;

    cl_int *particle_count_array; // Array of CVs with just the number of particles in each CV.
    cl_mem gparticle_count_array;

    cl_float cv_length;
    cl_int cvs_per_edge;
    cl_float domain_length = 3;
    cl_ulong NUMCVS; // Total number of CVs.

    cl_ulong NUMPART = 6;

    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_int ret;

    printf("\nTesting particle count assignment.\n");

    // Initializing OpenCL.
    setDevices(&platforms, &devices, FALSE);
    cl_context context = getContext(&devices, TRUE);
    cl_command_queue queue = getCommandQueue(&context, &devices, TRUE);
    cl_kernel assign_particle_count = getKernel(&devices, &context, "../kernels/assign_particles.cl",
                                                "assign_particle_count", TRUE);

    hparticles = malloc(sizeof(particle) * NUMPART);

    cl_float particle_diameter = 1;

    for (cl_ulong i = 0; i < 3; i++) {
        hparticles[i].id = i;
        hparticles[i].density = 2000;
        hparticles[i].fluid_viscosity = 0.0000193 * 100;
        hparticles[i].diameter = particle_diameter;
        hparticles[i].vel = (cl_float3) {0.0, 0.0, 0.0};
        hparticles[i].forces = (cl_float3) {0.0, 0.0, 0.0};
    }

    hparticles[0].pos = (cl_float3) {-1.4, -1.4, -1.4}; // 'minimum' CV.
    hparticles[1].pos = (cl_float3) {0, 0, 0}; // 'middle' CV.
    hparticles[2].pos = (cl_float3) {0, 0, 0}; // 'middle' CV.
    hparticles[3].pos = (cl_float3) {1.4, 1.4, 1.4}; // 'maximum' CV.
    hparticles[4].pos = (cl_float3) {1.4, 1.4, 1.4}; // 'maximum' CV.
    hparticles[5].pos = (cl_float3) {1.4, 1.4, 1.4}; // 'maximum' CV.

    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);
    ret = particlesToDevice(queue, gparticles, &hparticles, NUMPART);

    create_domain_count_cvs(&particle_count_array, &cv_length, &cvs_per_edge, domain_length, particle_diameter);
    NUMCVS = (cl_ulong) cvs_per_edge * cvs_per_edge * cvs_per_edge;

    gparticle_count_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * NUMCVS, NULL, &ret);
    ret = particle_count_arrayToDevice(queue, gparticle_count_array, &particle_count_array, NUMCVS);

    ret = clSetKernelArg(assign_particle_count, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(assign_particle_count, 1, sizeof(cl_mem), &gparticle_count_array);
    ret = clSetKernelArg(assign_particle_count, 2, sizeof(cl_float), &domain_length);
    ret = clSetKernelArg(assign_particle_count, 3, sizeof(cl_float), &cv_length);
    ret = clSetKernelArg(assign_particle_count, 4, sizeof(cl_int), &cvs_per_edge);
    ret = clEnqueueNDRangeKernel(queue, assign_particle_count, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);
    ret = clFinish(queue);

    ret = particle_count_arrayToHost(queue, gparticle_count_array, &particle_count_array, NUMCVS);

    if (NUMCVS != 27) {
        printf("Incorrect number of CVs.\n");
        return false;
    }

    if (particle_count_array[get_array_idx((cl_int3) {0, 0, 0}, cvs_per_edge)] != 1
        || particle_count_array[get_array_idx((cl_int3) {1, 1, 1}, cvs_per_edge)] != 2
        || particle_count_array[get_array_idx((cl_int3) {2, 2, 2}, cvs_per_edge)] != 3) {
        printf("Particles not correctly assigned to CVs.\n");
        return false;
    }

    return true;
}

boolean test_set_array_starts(boolean verbose) {
    cl_int particle_count_array[] = {3, 2, 1, 0, 6};
    cl_ulong NUMCVS = 5;
    cl_ulong *cv_array_starts;

    set_array_starts(particle_count_array, &cv_array_starts, NUMCVS);
    return (cv_array_starts[0] == 0
            && cv_array_starts[1] == 3
            && cv_array_starts[2] == 5
            && cv_array_starts[3] == -1
            && cv_array_starts[4] == 6);
}