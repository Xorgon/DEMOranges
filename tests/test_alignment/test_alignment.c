//
// Created by Elijah on 19/12/2017.
//

#include "test_alignment.h"
#include "../../structures/collision.h"

boolean test_particle_struct_alignment(boolean verbose) {
    particle *hparticles;
    cl_mem gparticles;
    cl_ulong NUMPART = 10;

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
        hparticles[i].id = 29;
        hparticles[i].diameter = 30;
        hparticles[i].density = 31;
        hparticles[i].fluid_viscosity = 32;
    }

    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);
    gcorrect = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(boolean), NULL, &ret);

    ret = particlesToDevice(queue, gparticles, &hparticles, NUMPART);
    ret = clEnqueueWriteBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &gcorrect);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, (size_t *) &NUMPART, 0, NULL, NULL, NULL);

    ret = particlesToHost(queue, gparticles, &hparticles, NUMPART);
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
              p.id == 89 &&
              p.diameter == 90 &&
              p.density == 91 &&
              p.fluid_viscosity == 92)) {
            hcorrect = FALSE;
        }
    }
    return (boolean) hcorrect;
}

boolean test_pp_collision_struct_alignment(boolean verbose) {
    pp_collision *hpp_collisions;
    cl_mem gpp_collisions;
    cl_ulong NUMCOLS = 10;

    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_int ret;
    cl_bool hcorrect = TRUE;
    cl_mem gcorrect;

    if (verbose) printf("\nTesting pp_collision struct alignment.\n");

    setDevices(&platforms, &devices, FALSE);
    cl_context context = getContext(&devices, verbose);
    cl_kernel kernel = getKernel(&devices, &context, "../tests/test_alignment/alignment_test_kernels.cl",
                                 "test_pp_collision_struct_alignment", verbose);
    cl_command_queue queue = getCommandQueue(&context, &devices, verbose);

    hpp_collisions = malloc(sizeof(pp_collision) * NUMCOLS);

    for (cl_ulong i = 0; i < NUMCOLS; i++) {
        hpp_collisions[i].p1_id = 20;
        hpp_collisions[i].p2_id = 21;
        hpp_collisions[i].stiffness = 22;
        hpp_collisions[i].damping_coefficient = 23;
        hpp_collisions[i].friction_coefficient = 24;
        hpp_collisions[i].friction_stiffness = 25;
    }

    gpp_collisions = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(pp_collision) * NUMCOLS, NULL, &ret);
    gcorrect = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(boolean), NULL, &ret);

    ret = pp_collisionsToDevice(queue, gpp_collisions, &hpp_collisions, NUMCOLS);
    ret = clEnqueueWriteBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gpp_collisions);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &gcorrect);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, (size_t *) &NUMCOLS, 0, NULL, NULL, NULL);

    ret = pp_collisionsToHost(queue, gpp_collisions, &hpp_collisions, NUMCOLS);
    ret = clEnqueueReadBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clFinish(queue);

    if (!hcorrect) {
        return FALSE;
    }
    for (int i = 0; i < NUMCOLS; i++) {
        pp_collision col = hpp_collisions[i];
        if (!(col.p1_id == 80 &&
              col.p2_id == 81 &&
              col.stiffness == 82 &&
              col.damping_coefficient == 83 &&
              col.friction_coefficient == 84 &&
              col.friction_stiffness == 85)) {
            hcorrect = FALSE;
        }
    }
    return (boolean) hcorrect;
}
//boolean test_wall_struct_alignment(boolean verbose);