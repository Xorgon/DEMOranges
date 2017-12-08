#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

#include "../util/clUtils.h"
#include "../structures/particle.h"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

particle *particles;
int NUMPART = 1000;

int main() {
    cl_platform_id *platforms;
    cl_device_id *devices;

    // Initializing OpenCL.
    setDevices(&platforms, &devices);
    cl_context context = getContext(&devices);
    cl_kernel kernel = getKernel(&devices, &context, "../clEnqueueNDRangeKernel/propagate.cl", "propagate");
    cl_command_queue queue = getCommandQueue(&context, &devices);

    particles = malloc(sizeof(particle) * NUMPART);
    cl_float density = 2000;
    cl_float particle_diameter = 0.1;
    cl_float fluid_viscosity = 0.0000193;

    for (cl_ulong i = 0; i < NUMPART; i++) {
        particles[i].particle_id = i;
        particles[i].density = density;
        particles[i].fluid_viscosity = fluid_viscosity;
        particles[i].particle_diameter = particle_diameter;
        particles[i].pos = (cl_float3) (1.0, 1.0, 1.0);
        particles[i].vel; // TODO: Define this stuff.
        particles[i].forces;

    }

}