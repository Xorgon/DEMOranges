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
#define LOG_COL_STATS TRUE

char *prefix = "TGV_PERIODIC";
char *log_dir[100];

particle *hparticles;
cl_ulong NUMPART = 10000;

// Particle properties.
cl_float density = 2000;
cl_float particle_diameter = 0.05;
cl_float particle_effect_diameter;
cl_float fluid_viscosity;

// Collision properties.
cl_float stiffness = 1e5;
cl_float restitution_coefficient = 0.01;
cl_float friction_coefficient = 0.1;
cl_float friction_stiffness = 1e5;
cl_float cohesion_stiffness = 1e2;

cl_ulong NUMWALLS;
aa_wall *walls;
cl_bool periodic = CL_TRUE;

float init_speed_mean = 1;
float init_speed_std_dev = 0.1;

cl_float timestep;
cl_float sim_length = 200;
cl_float log_step = 1;

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
                                      "../kernels/get_gravity/no_gravity.cl",
                                      "../kernels/get_vel_fluid/tgv.cl",
                                      "../kernels/iterate_particle.cl"};
    cl_kernel iterate_particle = getKernel(device, context, iterate_particle_files, 4, "iterate_particle", TRUE);

    hparticles = malloc(sizeof(particle) * NUMPART);
    if (hparticles == NULL) {
        fprintf(stderr, "Particles memory allocation failed.\n");
        return 1;
    }

    particle_effect_diameter = (cl_float) (1.5 * particle_diameter);

    float effect_diameters[] = {1.5f * particle_diameter, 1.5f * particle_diameter,
                                1.5f * particle_diameter, 1.5f * particle_diameter};
    float sys[] = {0.1, 1, 1.5, 0.6};
    float rests[] = {0.5, 0.173618, 0.15, 0.2};
    float stks[] = {0.1, 1, 10, 5};

    for (int i = -1; i < 3; i++) {
        for (int j = 1; j < 2; j++) {
            particle_effect_diameter = effect_diameters[i + 1];
            restitution_coefficient = rests[i + 1];
            cohesion_stiffness = getCohesionFromSy(effect_diameters[i + 1] - particle_diameter,
                                                   rests[i + 1], 0.7839 * 5,
                                                   get_particle_mass_from_values(density, particle_diameter),
                                                   sys[i + 1]);

            fluid_viscosity = getFluidViscFromStks(particle_diameter, density, 0.7839 * 5, PI / 3, stks[j + 1]);

            sprintf(log_dir, "TGV_PERIODIC_%i_%i/", i, j);

            printf("[INIT] Creating particle positions.\n");
            cl_float3 *positions = malloc(sizeof(cl_float3) * NUMPART);
            // Using particle_effect_diameter so that cohesion effects are considered at the appropriate range.
            float cube_length = createCubePositions(positions, NUMPART, particle_effect_diameter, 2,
                                                    (cl_float3) {0, 0, 0});
            domain_length = (cl_float) (2 * PI);

            cl_float3 *velocities = malloc(sizeof(cl_float3) * NUMPART);
            createNormalDistVelocities(velocities, NUMPART, init_speed_mean, init_speed_std_dev);

            // Initialize particles.
            initializeMonodisperseParticles(hparticles, NUMPART, density, fluid_viscosity, particle_diameter,
                                            particle_effect_diameter, positions, velocities);
            free(positions);
            free(velocities);

            if (!checkPositions(hparticles, NUMPART, domain_length)) {
                fprintf(stderr, "Particles outside domain limits.\n");
                return 1;
            }

            timestep = (cl_float) (PI * sqrt(get_particle_mass(&(hparticles[0])) / stiffness) / 8);

            writeSetupData(prefix, log_dir, NUMPART, timestep, sim_length, domain_length, stiffness,
                           restitution_coefficient, friction_coefficient, friction_stiffness, cohesion_stiffness,
                           particle_diameter, particle_effect_diameter, density, fluid_viscosity);

            if (restitution_coefficient > 1 || restitution_coefficient < 0) {
                printf("Restitution coefficient invalid. Skipping simulation.");
                continue;
            }

            int sim_ret = runSim(hparticles, NUMPART, iterate_particle, particle_diameter, NULL, 0, periodic, stiffness,
                                 restitution_coefficient, friction_coefficient, stiffness, cohesion_stiffness,
                                 domain_length, prefix, log_dir, sim_length, timestep, VERBOSE, LOG_DATA, FALSE,
                                 LOG_COL_STATS, log_step, device, context);

            clReleaseContext(context);
        }
    }
    return 0;
}