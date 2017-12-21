//
// Created by Elijah on 20/12/2017.
//

#include "test_atomics.h"
#include "../../structures/particle.h"

boolean test_atomics(boolean verbose) {
    particle *hparticles;
    cl_mem gparticles;
    cl_ulong NUMPART = 10;

    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_int ret;

    if (verbose) printf("\nTesting atomic float addition.\n");

    setDevices(&platforms, &devices, FALSE);
    cl_context context = getContext(&devices, verbose);
    cl_kernel kernel = getKernel(&devices, &context, "../tests/test_atomics/test_atomics.cl",
                                 "test_atomics", verbose);
    cl_command_queue queue = getCommandQueue(&context, &devices, verbose);

    hparticles = malloc(sizeof(particle) * NUMPART);

    for (cl_ulong i = 0; i < NUMPART; i++) {
        hparticles[i].pos = (cl_float3) {0, 0, 0};
        hparticles[i].vel = (cl_float3) {23, 24, 25};
        hparticles[i].forces = (cl_float3) {26, 27, 28};
        hparticles[i].id = 29;
        hparticles[i].diameter = 30;
        hparticles[i].density = 31;
        hparticles[i].fluid_viscosity = 32;
    }

    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);

    ret = particlesToDevice(queue, gparticles, &hparticles, NUMPART);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gparticles);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, (size_t *) &NUMPART, 0, NULL, NULL, NULL);

    ret = particlesToHost(queue, gparticles, &hparticles, NUMPART);

    ret = clFinish(queue);

    if (verbose) printf("Result = %f, %f, %f\n", hparticles[0].pos.x, hparticles[0].pos.y, hparticles[0].pos.z);
    return (boolean) (hparticles[0].pos.x == NUMPART &&
                      hparticles[0].pos.y == 2 * NUMPART &&
                      hparticles[0].pos.z == 3 * NUMPART);
}