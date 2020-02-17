#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

#include <stdio.h>
#include <malloc.h>
#include "../util/clUtils/clUtils.h"
#include "../util/particleUtils/particleUtils.h"
#include "../tests/run_tests/run_tests.h"
#include "../util/simUtils/simUtils.h"
#include "simRunner/simRunner.h"

#define MAX_SOURCE_SIZE (0x100000)
#define VERBOSE FALSE
#define LOG_DATA TRUE

char *prefix = "BALL";
char *log_dir = "BALL/";

particle *hparticles;
cl_ulong NUMPART = 10000;

cl_float density = 2000;
cl_float particle_diameter = 0.1;
cl_float fluid_viscosity = 0.0000193 * 100;

cl_float stiffness = 1e5;
cl_float restitution_coefficient = 0.8;
cl_float friction_coefficient = 0.6;
cl_float friction_stiffness = 1e5;
cl_float cohesion_stiffness = 0;

cl_float timestep = 0.0005;
cl_float sim_length = 10;
cl_float log_step = 0.0333;

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
                                      PROJECT_DIR "/kernels/get_gravity/point_gravity.cl",
                                      PROJECT_DIR "/kernels/iterate_particle.cl"};
    cl_kernel iterate_particle = getKernel(device, context, iterate_particle_files, 3, "iterate_particle", VERBOSE);

    hparticles = malloc(sizeof(particle) * NUMPART);
    if (hparticles == NULL) {
        fprintf(stderr, "Particles memory allocation failed.\n");
        return 1;
    }

    cl_float3 *positions = malloc(sizeof(cl_float3) * NUMPART);

    printf("[INIT] Creating particle positions.\n");
    float cube_length = createCubePositions(positions, NUMPART, particle_diameter, 1.2, (cl_float3) {0, 0, 0});
    domain_length = (cl_float) (1.25 * cube_length);
    initializeMonodisperseParticles(hparticles, NUMPART, density, fluid_viscosity, particle_diameter, 0, positions,
                                    NULL);
    free(positions);

    if (!checkPositions(hparticles, NUMPART, domain_length)) {
        fprintf(stderr, "Particles outside domain limits.\n");
        return 1;
    }

    if (!writeSetupData(prefix, log_dir, NUMPART, timestep, sim_length, domain_length, stiffness,
                        restitution_coefficient, friction_coefficient, friction_stiffness, cohesion_stiffness,
                        particle_diameter, 0, density, fluid_viscosity)) {
        return 1;
    }

    int sim_ret = runSim(hparticles, NUMPART, iterate_particle, particle_diameter, NULL, 0, FALSE, stiffness,
                         restitution_coefficient, friction_coefficient, stiffness, cohesion_stiffness, domain_length,
                         prefix, log_dir, sim_length, timestep, VERBOSE, LOG_DATA, FALSE, FALSE, log_step, device,
                         context);
    clReleaseContext(context);
    return sim_ret;
}