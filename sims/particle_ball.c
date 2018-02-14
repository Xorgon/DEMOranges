#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include "../util/clUtils.h"
#include "../util/particleUtils.h"
#include "../util/collisionUtils.h"
#include "../util/cvUtils.h"
#include "../structures/particle.h"
#include "../structures/collision.h"
#include "../tests/run_tests/run_tests.h"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)
#define verbose FALSE

particle *hparticles;
cl_mem gparticles;
cl_ulong NUMPART = 100000;

cl_mem gpp_cols;
cl_ulong NUMPPCOLS;

cl_float timestep = 0.0005;
cl_float sim_length = 10;
cl_float last_write = 0;
cl_float log_step = 0.0333;

cl_float cv_length;
cl_int cvs_per_edge;
cl_float domain_length;
cl_ulong NUMCVS; // Total number of CVs.

cl_int *particle_count_array; // Array of CVs with just the number of particles in each CV.
cl_int *input_count_array; // Array of CVs with a count of how many particles have been input for each CV.
cl_ulong *cv_start_array; // Array of CVs with the index of where the CV starts in cv_pids.
cl_ulong *cv_pids; // Array of particle IDs, sorted into CVs. Referenced with cv_start_array and particle_count_array.

// Device memory copies of the above arrays.
cl_mem gparticle_count_array;
cl_mem ginput_count_array;
cl_mem gcv_start_array;
cl_mem gcv_pids;

cl_context context;
cl_device_id device;
cl_int ret;

int main() {

    // Initializing OpenCL.
    setContext(&device, &context, TRUE);

    // Run tests
    if (!run_all_tests(device, context, FALSE)) {
        return 1;
    }

    cl_kernel iterate_particle = getKernel(device, context, "../kernels/iterate_particle.cl",
                                           "iterate_particle", verbose);
    cl_kernel calculate_pp_collision = getKernel(device, context, "../kernels/calculate_pp_collision.cl",
                                                 "calculate_pp_collision", verbose);
    cl_kernel assign_particle_count = getKernel(device, context, "../kernels/assign_particles.cl",
                                                "assign_particle_count", verbose);
    cl_kernel assign_particles = getKernel(device, context, "../kernels/assign_particles.cl",
                                           "assign_particles", verbose);
    cl_kernel count_pp_collisions = getKernel(device, context, "../kernels/make_pp_collisions.cl",
                                              "count_pp_collisions", verbose);
    cl_kernel make_pp_collisions = getKernel(device, context, "../kernels/make_pp_collisions.cl",
                                             "make_pp_collisions", verbose);
    cl_command_queue queue = getCommandQueue(context, device, verbose);

    hparticles = malloc(sizeof(particle) * NUMPART);
    if (hparticles == NULL) {
        fprintf(stderr, "Particles memory allocation failed.");
    }

    cl_float density = 2000;
    cl_float particle_diameter = 0.1;
    cl_float fluid_viscosity = 0.0000193 * 100;

    cl_float3 *positions = malloc(sizeof(cl_float3) * NUMPART);

    if (positions == NULL) {
        fprintf(stderr, "Position memory allocation failed.");
    }

    srand(0);
    printf("[INIT] Creating particle positions.\n");
    cl_ulong pos_len = 0;
    auto cubert_NUMPART = (cl_ulong) ceil(pow(NUMPART, 0.334));

    domain_length = (cl_float) (2 * 1.2 * cubert_NUMPART * particle_diameter);
    printf("[INIT] Domain length = %f\n", domain_length);

    for (int x = 0; x < cubert_NUMPART; x++) {
        for (int y = 0; y < cubert_NUMPART; y++) {
            for (int z = 0; z < cubert_NUMPART; z++) {
                if (pos_len < NUMPART) {
                    float rand_offset = 0.005 * (float) rand() / (float) (RAND_MAX);
                    cl_float xf = 1.2 * cubert_NUMPART * particle_diameter
                                  * (-0.5 + rand_offset + ((float) x / cubert_NUMPART));
                    cl_float yf = 1.2 * cubert_NUMPART * particle_diameter
                                  * (-0.5 + rand_offset + ((float) y / cubert_NUMPART));
                    cl_float zf = 1.2 * cubert_NUMPART * particle_diameter
                                  * (-0.5 + rand_offset + ((float) z / cubert_NUMPART));
                    positions[pos_len] = (cl_float3) {xf, yf, zf};
                }
                pos_len++;
            }
        }
    }
    for (cl_ulong i = 0; i < NUMPART; i++) {
        hparticles[i].id = i;
        hparticles[i].density = density;
        hparticles[i].fluid_viscosity = fluid_viscosity;
        hparticles[i].diameter = particle_diameter;
        hparticles[i].pos = positions[i];
        hparticles[i].vel = (cl_float3) {0.0, 0.0, 0.0};
        hparticles[i].forces = (cl_float3) {0.0, 0.0, 0.0};
    }

    free(positions);

    printf("[INIT] Creating CV arrays.\n");
    create_domain_count_cvs(&particle_count_array, &cv_length, &cvs_per_edge, domain_length, particle_diameter);
    printf("       - CVs per edge = %i\n", cvs_per_edge);
    NUMCVS = (cl_ulong) cvs_per_edge * cvs_per_edge * cvs_per_edge;
    printf("       - Total CVs = %lu\n", (unsigned long) NUMCVS);
    gparticle_count_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * NUMCVS, NULL, &ret);

    cv_start_array = malloc(sizeof(cl_ulong) * NUMCVS);
    gcv_start_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * NUMCVS, NULL, &ret);

    input_count_array = calloc(NUMCVS, sizeof(cl_int));
    ginput_count_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * NUMCVS, NULL, &ret);

    cv_pids = malloc(sizeof(cl_ulong) * NUMPART);
    gcv_pids = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * NUMPART, NULL, &ret);

    cl_float stiffness = 1e5;
    cl_float restitution_coefficient = 0.8;
    cl_float friction_coefficient = 0.6;
    cl_float friction_stiffness = 1e5;

    cl_ulong collision_count = 0;
    cl_mem gcollision_count = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong), NULL, &ret);

    printf("\nRunning sim with %i particles.\n", NUMPART);
    printf("Logging at time: 0.000000\n");
    writeParticles(hparticles, 0, "TEST", "", NUMPART);

    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);
    ret = clSetKernelArg(iterate_particle, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(iterate_particle, 1, sizeof(cl_float), &timestep);

    ret = clSetKernelArg(calculate_pp_collision, 0, sizeof(cl_mem), &gpp_cols);
    ret = clSetKernelArg(calculate_pp_collision, 1, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(calculate_pp_collision, 2, sizeof(cl_float), &timestep);
    ret = clSetKernelArg(calculate_pp_collision, 3, sizeof(cl_float), &stiffness);
    ret = clSetKernelArg(calculate_pp_collision, 4, sizeof(cl_float), &restitution_coefficient);
    ret = clSetKernelArg(calculate_pp_collision, 5, sizeof(cl_float), &friction_coefficient);
    ret = clSetKernelArg(calculate_pp_collision, 6, sizeof(cl_float), &friction_stiffness);

    ret = particlesToDevice(queue, gparticles, &hparticles, NUMPART);

    ret = clSetKernelArg(assign_particle_count, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(assign_particle_count, 1, sizeof(cl_mem), &gparticle_count_array);
    ret = clSetKernelArg(assign_particle_count, 2, sizeof(cl_float), &domain_length);
    ret = clSetKernelArg(assign_particle_count, 3, sizeof(cl_float), &cv_length);
    ret = clSetKernelArg(assign_particle_count, 4, sizeof(cl_int), &cvs_per_edge);

    ret = clSetKernelArg(assign_particles, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(assign_particles, 1, sizeof(cl_mem), &gcv_start_array);
    ret = clSetKernelArg(assign_particles, 2, sizeof(cl_mem), &ginput_count_array);
    ret = clSetKernelArg(assign_particles, 3, sizeof(cl_mem), &gcv_pids);

    clSetKernelArg(count_pp_collisions, 0, sizeof(cl_mem), &gparticle_count_array);
    clSetKernelArg(count_pp_collisions, 1, sizeof(cl_int), &cvs_per_edge);
    clSetKernelArg(count_pp_collisions, 2, sizeof(cl_mem), &gcollision_count);

    clSetKernelArg(make_pp_collisions, 0, sizeof(cl_mem), &gcv_start_array);
    clSetKernelArg(make_pp_collisions, 1, sizeof(cl_mem), &gparticle_count_array);
    clSetKernelArg(make_pp_collisions, 2, sizeof(cl_mem), &gcv_pids);
    clSetKernelArg(make_pp_collisions, 3, sizeof(cl_int), &cvs_per_edge);
    clSetKernelArg(make_pp_collisions, 4, sizeof(cl_mem), &gpp_cols);
    clSetKernelArg(make_pp_collisions, 5, sizeof(cl_mem), &gcollision_count);

    for (cl_float time = timestep; time <= sim_length; time += timestep) {
        if (verbose) printf("Time = %f\n", time);
        // Count how many particles are in each CV.
        if (verbose) printf("    Counting particles per CV\n");
        memset(particle_count_array, 0, sizeof(cl_int) * NUMCVS); // Reset counts to 0.
        ret = intArrayToDevice(queue, gparticle_count_array, &particle_count_array, NUMCVS);
        ret = clEnqueueNDRangeKernel(queue, assign_particle_count, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);

        // Set CV array starts according to the counts and assign particles to the cv_pid array.
        ret = intArrayToHost(queue, gparticle_count_array, &particle_count_array, NUMCVS);
        set_array_starts(particle_count_array, cv_start_array, NUMCVS);
        ulongArrayToDevice(queue, gcv_start_array, &cv_start_array, NUMCVS);
        memset(input_count_array, 0, sizeof(cl_int) * NUMCVS); // Reset counts to 0.
        intArrayToDevice(queue, ginput_count_array, &input_count_array, NUMCVS);
        if (verbose) printf("    Assigning particles to CVs\n");
        ret = clEnqueueNDRangeKernel(queue, assign_particles, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);

        // Count collisions.
        if (verbose) printf("    Counting collisions\n");
        collision_count = 0;
        clEnqueueWriteBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);
        ret = clEnqueueNDRangeKernel(queue, count_pp_collisions, 1, NULL, &NUMCVS, 0, NULL, NULL, NULL);
        clEnqueueReadBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);
        NUMPPCOLS = collision_count;

        // Make collisions.
        if (verbose) printf("    Making %llu collisions\n", collision_count);
        ret = clReleaseMemObject(gpp_cols);
        gpp_cols = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(pp_collision) * NUMPPCOLS, NULL, &ret);

        collision_count = 0;
        clEnqueueWriteBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);

        clSetKernelArg(make_pp_collisions, 4, sizeof(cl_mem), &gpp_cols);

        clEnqueueNDRangeKernel(queue, make_pp_collisions, 1, NULL, &NUMCVS, 0, NULL, NULL, NULL);

        // Calculate collisions.
        if (verbose) printf("    Calculating collisions\n");
        ret = clSetKernelArg(calculate_pp_collision, 0, sizeof(cl_mem), &gpp_cols);
        ret = clEnqueueNDRangeKernel(queue, calculate_pp_collision, 1, NULL, &NUMPPCOLS, 0, NULL, NULL, NULL);

        // Iterate particles.
        if (verbose) printf("    Iterating particles\n");
        ret = clEnqueueNDRangeKernel(queue, iterate_particle, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);
        if (time - last_write > log_step) {
            ret = particlesToHost(queue, gparticles, &hparticles, NUMPART);
            printf("Logging at time: %f\n", time);
            writeParticles(hparticles, time, "TEST", "", NUMPART);

            last_write = time;
        }
    }
}