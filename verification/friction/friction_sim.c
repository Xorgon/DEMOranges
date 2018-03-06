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
#include "../../util/wallUtils/wallUtils.h"
#include "../../structures/wall.h"
#include <math.h>

#define MAX_SOURCE_SIZE (0x100000)
#define VERBOSE FALSE
#define LOG_DATA TRUE

char prefix[50];

particle *hparticles;
cl_ulong NUMPART = 1;

aa_wall *hwalls;
cl_ulong NUMWALLS = 1;

cl_float density = 2000;
cl_float particle_diameter = 0.1;
cl_float fluid_viscosity = 0.0000193 * 100;

cl_float stiffness = 1e5;
cl_float restitution_coefficient = 0.8;
cl_float friction_coefficient = 0.6;
cl_float friction_stiffness = 1e9;
cl_float cohesion_stiffness = 0;

cl_float initial_vel_x = 1;

cl_float timestep;
cl_float sim_length = 2.5;
cl_float log_step = 0.03125;

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
                                      "../kernels/get_vel_fluid/no_drag.cl",
                                      "../kernels/iterate_particle.cl"};
    cl_kernel iterate_particle = getKernel(device, context, iterate_particle_files, 3, "iterate_particle", TRUE);

    hparticles = malloc(sizeof(particle) * NUMPART);

    hparticles[0].diameter = particle_diameter;
    hparticles[0].effect_diameter = 0;
    hparticles[0].fluid_viscosity = fluid_viscosity;
    hparticles[0].density = density;
    hparticles[0].id = 0;
    hparticles[0].pos = (cl_float3) {0, 0, 0};
    hparticles[0].vel = (cl_float3) {initial_vel_x, 0, 0};
    hparticles[0].forces = (cl_float3) {0, 0, 0};

    hwalls = malloc(sizeof(aa_wall) * NUMWALLS);
    domain_length = 1;
    hwalls[0].min = (cl_float3) {-domain_length / 2, 0, -domain_length / 2};
    hwalls[0].max = (cl_float3) {domain_length / 2, 0, domain_length / 2};
    hwalls[0].normal = get_aa_wall_normal(hwalls[0]);

    float collision_time = 3.14159 * sqrt(get_particle_mass(&(hparticles[0])) / stiffness);
    printf("Mass = %f\n", get_particle_mass(&(hparticles[0])));
    printf("Collision Duration = %f\n", collision_time);

    float theoretical_overlap = get_particle_mass(&(hparticles[0])) * 9.81 / stiffness;
    printf("Theoretical Overlap = %f\n", theoretical_overlap);

    char dir[] = "../verification/friction/data/";
    for (int i = 8; i <= 64; i *= 2) {
        timestep = collision_time / i;

        sprintf(prefix, "friction_%i", i);

        hparticles[0].pos = (cl_float3) {0, particle_diameter / 2 - theoretical_overlap, 0};
        hparticles[0].vel = (cl_float3) {initial_vel_x, 0, 0};

        runSim(hparticles, NUMPART, iterate_particle, particle_diameter, hwalls, NUMWALLS, stiffness,
               restitution_coefficient, friction_coefficient, stiffness, cohesion_stiffness, domain_length,
               prefix, dir, sim_length, timestep, VERBOSE, LOG_DATA, TRUE, log_step, device, context);
    }
}