#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "../util/clUtils.h"
#include "../util/particleUtils.h"
#include "../structures/particle.h"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

particle *hparticles;
cl_mem gparticles;
cl_ulong NUMPART = 1000;

cl_float timestep = 5e-4;
cl_float sim_length = 10;
cl_float last_write = 0;
cl_float log_step = 0.1;

cl_platform_id *platforms;
cl_device_id *devices;
cl_int ret;

int main() {

    // Initializing OpenCL.
    setDevices(&platforms, &devices, TRUE);
    cl_context context = getContext(&devices, TRUE);
    cl_kernel kernel = getKernel(&devices, &context, "../kernels/iterate_particle.cl", "iterate_particle", TRUE);
    cl_command_queue queue = getCommandQueue(&context, &devices, TRUE);

    hparticles = malloc(sizeof(particle) * NUMPART);
    cl_float density = 2000;
    cl_float particle_diameter = 0.1;
    cl_float fluid_viscosity = 0.0000193;

    for (cl_ulong i = 0; i < NUMPART; i++) {
        hparticles[i].id = i;
        hparticles[i].density = density;
        hparticles[i].fluid_viscosity = fluid_viscosity;
        hparticles[i].diameter = particle_diameter;
        hparticles[i].pos = (cl_float3) {0.0, i + 10, 0.0};
        hparticles[i].vel = (cl_float3) {0.0, 0.0, 0.0};
        hparticles[i].forces = (cl_float3) {0.0, 0.0, 0.0};
    }

    writeParticles(hparticles, 0, "TEST", "", NUMPART);

    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);

    for (cl_float time = timestep; time <= sim_length; time += timestep) {
        ret = particlesToDevice(queue, gparticles, &hparticles, NUMPART);
        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gparticles);
        ret = clSetKernelArg(kernel, 1, sizeof(cl_float), &timestep);
        ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, (size_t *) &NUMPART, 0, NULL, NULL, NULL);
        ret = particlesToHost(queue, gparticles, &hparticles, NUMPART);
        ret = clFinish(queue);
        if (time - last_write > log_step) {
            printf("Logging at time: %f\n", time);
            writeParticles(hparticles, time, "TEST", "", NUMPART);
            last_write = time;
        }
    }


}