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
#include <malloc.h>

#define MAX_SOURCE_SIZE (0x100000)
#define VERBOSE FALSE
#define LOG_DATA TRUE

char prefix[50];

particle *hparticles;
cl_ulong NUMPART = 2;

cl_float density = 2000;
cl_float particle_diameter = 0.1;
cl_float effect_diameter;
cl_float fluid_viscosity = 0.0000193 * 100;

cl_float stiffness = 1e5;
cl_float restitution_coefficient = 0.8;
cl_float friction_coefficient = 0.6;
cl_float friction_stiffness = 1e5;
cl_float cohesion_stiffness = 1e2;

cl_float initial_vel_x = -2;

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

    char *iterate_particle_files[] = {PROJECT_DIR "/util/kernelUtils.cl",
                                      PROJECT_DIR "/kernels/get_gravity/no_gravity.cl",
                                      PROJECT_DIR "/kernels/get_vel_fluid/no_drag.cl",
                                      PROJECT_DIR "/kernels/iterate_particle.cl"};
    cl_kernel iterate_particle = getKernel(device, context, iterate_particle_files, 4, "iterate_particle", TRUE);

    effect_diameter = 1.5f * particle_diameter;

    hparticles = malloc(sizeof(particle) * NUMPART);

    hparticles[0].diameter = particle_diameter;
    hparticles[0].effect_diameter = effect_diameter;
    hparticles[0].fluid_viscosity = fluid_viscosity;
    hparticles[0].density = -1;
    hparticles[0].id = 0;
    hparticles[0].pos = (cl_float3) {0, 0, 0};
    hparticles[0].vel = (cl_float3) {0, 0, 0};
    hparticles[0].forces = (cl_float3) {0, 0, 0};

    hparticles[1].diameter = particle_diameter;
    hparticles[1].effect_diameter = effect_diameter;
    hparticles[1].fluid_viscosity = fluid_viscosity;
    hparticles[1].density = density;
    hparticles[1].id = 1;
    hparticles[1].pos = (cl_float3) {effect_diameter, 0, 0};
    hparticles[1].vel = (cl_float3) {initial_vel_x, 0, 0};
    hparticles[1].forces = (cl_float3) {0, 0, 0};

    printf("TEST %f\n", effect_diameter);

    domain_length = 1;
    float collision_time = 3.14159 * sqrt(get_particle_mass(&(hparticles[1])) / stiffness);
    printf("Mass = %f\n", get_particle_mass(&(hparticles[1])));
    printf("Collision Duration = %f\n", collision_time);
    sim_length = (cl_float) (collision_time * 4);

    char dir[] = PROJECT_DIR "/verification/cohesion/data/";
    for (int i = 8; i <= 64; i *= 2) {
        timestep = collision_time / i;
        log_step = timestep;

        sprintf(prefix, "cohesion_%i", i);

        hparticles[0].pos = (cl_float3) {0, 0, 0};
        hparticles[0].vel = (cl_float3) {0, 0, 0};

        hparticles[1].pos = (cl_float3) {effect_diameter, 0, 0};
        hparticles[1].vel = (cl_float3) {initial_vel_x, 0, 0};

        runSim(hparticles, NUMPART, iterate_particle, effect_diameter, NULL, 0, FALSE, stiffness,
               restitution_coefficient, friction_coefficient, friction_stiffness, cohesion_stiffness, domain_length,
               prefix, dir, sim_length, timestep, VERBOSE, LOG_DATA, TRUE, FALSE, log_step, device, context);
    }
}