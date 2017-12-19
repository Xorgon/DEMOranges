//
// Created by Elijah on 19/12/2017.
//

#include "test_alignment.h"

boolean test_particle_struct_alignment(boolean verbose) {
    particle *hparticles;
    cl_mem gparticles;
    int NUMPART = 10;

    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_int ret;
    cl_bool hcorrect = TRUE;
    cl_mem gcorrect;

    if (verbose) printf("\nTesting particle struct alignment.\n");

    setDevices(&platforms, &devices, FALSE);
    cl_context context = getContext(&devices, verbose);
    cl_kernel kernel = getKernel(&devices, &context, "../tests/test_alignment/alignment_test_kernels.cl",
                                 "test_particle_struct_alignment", verbose);
    cl_command_queue queue = getCommandQueue(&context, &devices, verbose);

    hparticles = malloc(sizeof(particle) * NUMPART);

    for (cl_ulong i = 0; i < NUMPART; i++) {
        hparticles[i].pos = (cl_float3) {20, 21, 22};
        hparticles[i].vel = (cl_float3) {23, 24, 25};
        hparticles[i].forces = (cl_float3) {26, 27, 28};
        hparticles[i].particle_id = 29;
        hparticles[i].particle_diameter = 30;
        hparticles[i].density = 31;
        hparticles[i].fluid_viscosity = 32;
    }

    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);
    gcorrect = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(boolean), NULL, &ret);

    ret = clEnqueueWriteBuffer(queue, gparticles, CL_TRUE, 0, sizeof(particle) * NUMPART, hparticles, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &gcorrect);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, (size_t *) &NUMPART, 0, NULL, NULL, NULL);

    ret = clEnqueueReadBuffer(queue, gparticles, CL_TRUE, 0, sizeof(particle) * NUMPART, hparticles, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clFinish(queue);

    if (!hcorrect) {
        return FALSE;
    }
    for (int i = 0; i < NUMPART; i++) {
        particle p = hparticles[i];
        if (!(p.pos.x == 80 &&
              p.pos.y == 81 &&
              p.pos.z == 82 &&
              p.vel.x == 83 &&
              p.vel.y == 84 &&
              p.vel.z == 85 &&
              p.forces.x == 86 &&
              p.forces.y == 87 &&
              p.forces.z == 88 &&
              p.particle_id == 89 &&
              p.particle_diameter == 90 &&
              p.density == 91 &&
              p.fluid_viscosity == 92)) {
            hcorrect = FALSE;
        }
    }
    return (boolean) hcorrect;
}

//boolean test_collision_struct_alignment(boolean verbose);
//boolean test_wall_struct_alignment(boolean verbose);