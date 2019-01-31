#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

#include <stdio.h>
#include "../../util/clUtils/clUtils.h"
#include "../../util/particleUtils/particleUtils.h"
#include "../../tests/run_tests/run_tests.h"
#include "../../util/simUtils/simUtils.h"
#include "../../sims/simRunner/simRunner.h"
#include <math.h>

#define MAX_SOURCE_SIZE (0x100000)
#define VERBOSE FALSE
#define LOG_DATA TRUE

char prefix[50];

particle *hparticles;
cl_ulong NUMPART = 1;

cl_float density = 10;
cl_float particle_diameter = 0.1;
cl_float fluid_viscosity = 0.00193;

cl_float stiffness = 1e5;
cl_float restitution_coefficient = 0.8;
cl_float friction_coefficient = 0.6;
cl_float friction_stiffness = 1e5;
cl_float cohesion_stiffness = 0;

cl_float initial_vel_x = 0;

cl_float timestep;
cl_float sim_length;
cl_float log_step;

cl_float domain_length;

cl_context context;
cl_device_id device;

int main() {

    // Initializing OpenCL.
    setContext(&device, &context, TRUE);

    // Run tests
    if (!run_all_tests(device, context, FALSE)) {
        return 1;
    }

    char *iterate_particle_files[] = {"../util/kernelUtils.cl",
                                      "../kernels/get_gravity/no_gravity.cl",
                                      "../kernels/get_vel_fluid/x_1_drag.cl",
                                      "../kernels/iterate_particle.cl"};
    cl_kernel iterate_particle = getKernel(device, context, iterate_particle_files, 4, "iterate_particle", TRUE);

    hparticles = malloc(sizeof(particle) * NUMPART);

    hparticles[0].diameter = particle_diameter;
    hparticles[0].effect_diameter = 0;
    hparticles[0].fluid_viscosity = fluid_viscosity;
    hparticles[0].density = density;
    hparticles[0].id = 0;
    hparticles[0].pos = (cl_float3) {0, 0, 0};
    hparticles[0].vel = (cl_float3) {initial_vel_x, 0, 0};
    hparticles[0].forces = (cl_float3) {0, 0, 0};

    float tau = get_tau(&(hparticles[0]));
    sim_length = 5 * tau;

    domain_length = 1;
    printf("Mass = %f\n", get_particle_mass(&(hparticles[0])));
    printf("Tau = %f\n", tau);

    char dir[] = "../verification/drag/data/";
    for (int i = 8; i <= 64; i *= 2) {
        timestep = tau / i;
        log_step = timestep;

        sprintf(prefix, "drag_%i", i);

        hparticles[0].pos = (cl_float3) {0, 0, 0};
        hparticles[0].vel = (cl_float3) {0, 0, 0};

        runSim(hparticles, NUMPART, iterate_particle, particle_diameter, NULL, 0, FALSE, stiffness,
               restitution_coefficient, friction_coefficient, friction_stiffness, cohesion_stiffness, domain_length,
               prefix, dir, sim_length, timestep, VERBOSE, LOG_DATA, TRUE, FALSE, log_step, device, context);
    }
}