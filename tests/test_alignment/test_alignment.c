//
// Created by Elijah on 19/12/2017.
//

#include "test_alignment.h"
#include "../../structures/collision.h"
#include "../../structures/wall.h"

boolean test_particle_struct_alignment(cl_device_id device, cl_context context, boolean verbose) {
    particle *hparticles;
    cl_mem gparticles;
    cl_ulong NUMPART = 10;

    cl_int ret;
    cl_bool hcorrect = TRUE;
    cl_mem gcorrect;

    if (verbose) printf("\nTesting particle struct alignment.\n");

    cl_kernel kernel = getKernelWithUtils(device, context, PROJECT_DIR "/tests/test_alignment/alignment_test_kernels.cl",
                                          "test_particle_struct_alignment", verbose);
    cl_command_queue queue = getCommandQueue(context, device, verbose);

    hparticles = malloc(sizeof(particle) * NUMPART);

    for (cl_ulong i = 0; i < NUMPART; i++) {
        hparticles[i].pos = (cl_float3) {20, 21, 22};
        hparticles[i].vel = (cl_float3) {23, 24, 25};
        hparticles[i].forces = (cl_float3) {26, 27, 28};
        hparticles[i].id = 29;
        hparticles[i].cv_array_idx = 30;
        hparticles[i].diameter = 31;
        hparticles[i].effect_diameter = 32;
        hparticles[i].density = 33;
        hparticles[i].fluid_viscosity = 34;
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
              p.cv_array_idx == 90 &&
              p.diameter == 91 &&
              p.effect_diameter == 92 &&
              p.density == 93 &&
              p.fluid_viscosity == 94)) {
            hcorrect = FALSE;
        }
    }
    return (boolean) hcorrect;
}

boolean test_pp_collision_struct_alignment(cl_device_id device, cl_context context, boolean verbose) {
    pp_collision *hpp_collisions;
    cl_mem gpp_collisions;
    cl_ulong NUMCOLS = 10;

    cl_int ret;
    cl_bool hcorrect = TRUE;
    cl_mem gcorrect;

    if (verbose) printf("\nTesting pp_collision struct alignment.\n");

    cl_kernel kernel = getKernelWithUtils(device, context, PROJECT_DIR "/tests/test_alignment/alignment_test_kernels.cl",
                                          "test_pp_collision_struct_alignment", verbose);
    cl_command_queue queue = getCommandQueue(context, device, verbose);

    hpp_collisions = malloc(sizeof(pp_collision) * NUMCOLS);

    for (cl_ulong i = 0; i < NUMCOLS; i++) {
        hpp_collisions[i].p1_id = 20;
        hpp_collisions[i].p2_id = 21;
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
        printf("hcorrect is false after kernel execution (test_pp_collision_struct_alignment).");
        return FALSE;
    }
    for (int i = 0; i < NUMCOLS; i++) {
        pp_collision col = hpp_collisions[i];
        if (!(col.p1_id == 80
              && col.p2_id == 81
        )) {
            hcorrect = FALSE;
        }
    }
    return (boolean) hcorrect;
}

boolean test_pw_collision_struct_alignment(cl_device_id device, cl_context context, boolean verbose) {
    pw_collision *hpw_collisions;
    cl_mem gpw_collisions;
    cl_ulong NUMCOLS = 10;

    cl_int ret;
    cl_bool hcorrect = TRUE;
    cl_mem gcorrect;

    if (verbose) printf("\nTesting pw_collision struct alignment.\n");

    cl_kernel kernel = getKernelWithUtils(device, context, PROJECT_DIR "/tests/test_alignment/alignment_test_kernels.cl",
                                          "test_pw_collision_struct_alignment", verbose);
    cl_command_queue queue = getCommandQueue(context, device, verbose);

    hpw_collisions = malloc(sizeof(pw_collision) * NUMCOLS);

    for (cl_ulong i = 0; i < NUMCOLS; i++) {
        hpw_collisions[i].p_id = 20;
        hpw_collisions[i].w_id = 21;
    }

    gpw_collisions = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(pw_collision) * NUMCOLS, NULL, &ret);
    gcorrect = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(boolean), NULL, &ret);

    ret = pw_collisionsToDevice(queue, gpw_collisions, &hpw_collisions, NUMCOLS);
    ret = clEnqueueWriteBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gpw_collisions);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &gcorrect);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, (size_t *) &NUMCOLS, 0, NULL, NULL, NULL);

    ret = pw_collisionsToHost(queue, gpw_collisions, &hpw_collisions, NUMCOLS);
    ret = clEnqueueReadBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clFinish(queue);

    if (!hcorrect) {
        return FALSE;
    }
    for (int i = 0; i < NUMCOLS; i++) {
        pw_collision col = hpw_collisions[i];
        if (!(col.p_id == 80
              && col.w_id == 81
        )) {
            hcorrect = FALSE;
        }
    }
    return (boolean) hcorrect;
}

boolean test_aa_wall_struct_alignment(cl_device_id device, cl_context context, boolean verbose) {
    aa_wall *haa_walls;
    cl_mem gaa_walls;
    cl_ulong NUMWALLS = 10;

    cl_int ret;
    cl_bool hcorrect = TRUE;
    cl_mem gcorrect;

    if (verbose) printf("\nTesting aa_wall struct alignment.\n");

    cl_kernel kernel = getKernelWithUtils(device, context, PROJECT_DIR "/tests/test_alignment/alignment_test_kernels.cl",
                                          "test_aa_wall_struct_alignment", verbose);
    cl_command_queue queue = getCommandQueue(context, device, verbose);

    haa_walls = malloc(sizeof(aa_wall) * NUMWALLS);

    for (cl_ulong i = 0; i < NUMWALLS; i++) {
        haa_walls[i].normal = (cl_float3) {20, 21, 22};
        haa_walls[i].max = (cl_float3) {23, 24, 25};
        haa_walls[i].min = (cl_float3) {26, 27, 28};
    }

    gaa_walls = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(aa_wall) * NUMWALLS, NULL, &ret);
    gcorrect = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(boolean), NULL, &ret);

    ret = aa_wallsToDevice(queue, gaa_walls, &haa_walls, NUMWALLS);
    ret = clEnqueueWriteBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gaa_walls);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &gcorrect);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, (size_t *) &NUMWALLS, 0, NULL, NULL, NULL);

    aa_wallsToHost(queue, gaa_walls, &haa_walls, NUMWALLS);
    ret = clEnqueueReadBuffer(queue, gcorrect, CL_TRUE, 0, sizeof(boolean), &hcorrect, 0, NULL, NULL);

    ret = clFinish(queue);

    if (!hcorrect) {
        return FALSE;
    }
    for (int i = 0; i < NUMWALLS; i++) {
        aa_wall wall = haa_walls[i];
        if (!(wall.normal.x == 80
              && wall.normal.y == 81
              && wall.normal.z == 82
              && wall.max.x == 83
              && wall.max.y == 84
              && wall.max.z == 85
              && wall.min.x == 86
              && wall.min.y == 87
              && wall.min.z == 88
        )) {
            hcorrect = FALSE;
        }
    }
    return (boolean) hcorrect;
}