#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

#include <stdio.h>
#include "../util/clUtils/clUtils.h"
#include "../util/particleUtils/particleUtils.h"
#include "../tests/run_tests/run_tests.h"
#include "../util/wallUtils/wallUtils.h"
#include "../util/simUtils/simUtils.h"
#include "simRunner/simRunner.h"

#define MAX_SOURCE_SIZE (0x100000)
#define VERBOSE FALSE
#define LOG_DATA TRUE

char *prefix = "HOURGLASS";
char *log_dir = "HOURGLASS/";

particle *hparticles;
cl_ulong NUMPART = 250000;

// Particle properties.
cl_float density = 2000;
cl_float particle_diameter = 0.05;
cl_float fluid_viscosity = 0.0000193 * 100;

// Collision properties.
cl_float stiffness = 1e5;
cl_float restitution_coefficient = 0.4;
cl_float friction_coefficient = 0.6;
cl_float friction_stiffness = 1e5;
cl_float cohesion_stiffness = 0;

cl_ulong NUMWALLS;
aa_wall *walls;

cl_float timestep;
cl_float sim_length = 300;
cl_float log_step = 0.0333;

cl_float domain_length;

cl_context context;
cl_device_id device;

int main() {
    // Initialize OpenCL.
    setContext(&device, &context, TRUE);

    // Run tests
    if (!run_all_tests(device, context, FALSE)) {
        return 1;
    }

    // Build iterate_particle kernel.
    char *iterate_particle_files[] = {"../util/kernelUtils.cl",
                                      "../kernels/get_gravity/diag_gravity.cl",
                                      "../kernels/iterate_particle.cl"};
    cl_kernel iterate_particle = getKernel(device, context, iterate_particle_files, 3, "iterate_particle", VERBOSE);

    hparticles = malloc(sizeof(particle) * NUMPART);
    if (hparticles == NULL) {
        fprintf(stderr, "Particles memory allocation failed.\n");
        return 1;
    }

    printf("[INIT] Creating particle positions.\n");
    float spacing_fact = 1.2;
    float cube_center_offset = 0.5 * spacing_fact * ceil(pow(NUMPART, 0.334)) * particle_diameter + particle_diameter;

    cl_float3 *positions = malloc(sizeof(cl_float3) * NUMPART);
    cl_float3 particle_cube_center = (cl_float3) {cube_center_offset, cube_center_offset, 0};
    float cube_length = createCubePositions(positions, NUMPART, particle_diameter, spacing_fact, particle_cube_center);
    domain_length = (cl_float) (3 * cube_length);

    // Initialize particles.
    initializeMonodisperseParticles(hparticles, NUMPART, density, fluid_viscosity, particle_diameter, 0, positions,
                                    NULL);
    free(positions);

    if (!checkPositions(hparticles, NUMPART, domain_length)) {
        fprintf(stderr, "Particles outside domain limits.\n");
        return 1;
    }

    printf("[INIT] Generating walls\n");
    NUMWALLS = (cl_ulong) generate_hourglass(&walls, 3 * cube_length * sqrtf(2), 4 * particle_diameter);

//    print_walls(walls, NUMWALLS);

    timestep = (cl_float) (PI * sqrt(get_particle_mass(&(hparticles[0])) / stiffness) / 16);

    writeSetupData(prefix, log_dir, NUMPART, timestep, sim_length, domain_length, stiffness, restitution_coefficient,
                   friction_coefficient, friction_stiffness, cohesion_stiffness, particle_diameter, 0, density,
                   fluid_viscosity);

    int sim_ret = runSim(hparticles, NUMPART, iterate_particle, particle_diameter, walls, NUMWALLS, FALSE, stiffness,
                         restitution_coefficient, friction_coefficient, stiffness, cohesion_stiffness, domain_length,
                         prefix, log_dir,
                         sim_length, timestep, VERBOSE, LOG_DATA, FALSE, FALSE, log_step, device, context);

    clReleaseContext(context);
    return sim_ret;
}