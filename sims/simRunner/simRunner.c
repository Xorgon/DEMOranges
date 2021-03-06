//
// Created by Elijah on 03/03/2018.
//

#include "simRunner.h"


int runSim(particle *hparticles, cl_ulong NUMPART, cl_kernel iterate_particle, cl_float particle_diameter,
           aa_wall *walls, cl_ulong NUMWALLS, cl_bool periodic,
           cl_float stiffness, cl_float restitution_coefficient, cl_float friction_coefficient,
           cl_float friction_stiffness, cl_float cohesion_stiffness,
           cl_float domain_length, char prefix[], char log_dir[], float sim_length, float timestep,
           boolean VERBOSE, boolean LOG_DATA, boolean log_vel, boolean log_col_stats, float log_step,
           cl_device_id device, cl_context context) {
    cl_int ret;  // Variable in which to store OpenCL return values.

    cl_float cv_length;  // Side length of CVs.
    cl_int cvs_per_edge;  // The number of CVs along one edge of the domain.
    cl_ulong NUMCVS; // Total number of CVs.

    cl_int *particle_count_array; // Array of CVs with the number of particles in each CV.
    cl_int *input_count_array; // Array of CVs with a count of how many particles have been input for each CV.
    cl_ulong *cv_start_array; // Array of CVs with the index of where the CV starts in cv_pids.

    cl_ulong NUMPPCOLS;  // Variable to store total number of particle-particle collisions.
    cl_ulong collision_count = 0;  // Variable to count up the number of collisions.

    cl_ulong NUMPWCOLS;  // Variable to store total number of particle-wall collisions.
    pw_collision *hpw_cols;  // Array in which to store particle-wall collisions on the host device.

    cl_mem gparticle_count_array;  // GPU version of particle_count_array.
    cl_mem ginput_count_array; // GPU version of input_count_array.
    cl_mem gcv_start_array; // GPU version of cv_start_array.
    cl_mem gcv_pids;  // GPU array of the particle IDs sorted into control volumes.
    cl_mem gparticles;  // GPU array of particles.
    cl_mem gwalls;  // GPU array of walls.
    cl_mem gpw_cols;  // GPU version of hpw_cols.
    cl_mem gpp_cols = 0; // Set to 0 to avoid uninitialized errors.
    cl_mem gcollision_count;  // GPU version of collision_count.


    // Check that the logging directory exists if needed.
    if (!checkDirExists(log_dir) && LOG_DATA) {
        fprintf(stderr, "Error: Directory (%s) does not exist or cannot be accessed.\n", log_dir);
        return 1;
    }

    // Build all remaining kernels and create command queue.
    cl_kernel calculate_pp_collision = getKernelWithUtils(device, context,
                                                          PROJECT_DIR "/kernels/calculate_pp_collision.cl",
                                                          "calculate_pp_collision", VERBOSE);
    cl_kernel calculate_pw_collision = getKernelWithUtils(device, context,
                                                          PROJECT_DIR "/kernels/calculate_pw_collision.cl",
                                                          "calculate_pw_collision", VERBOSE);
    cl_kernel assign_particle_count = getKernelWithUtils(device, context, PROJECT_DIR "/kernels/assign_particles.cl",
                                                         "assign_particle_count", VERBOSE);
    cl_kernel assign_particles = getKernelWithUtils(device, context, PROJECT_DIR "/kernels/assign_particles.cl",
                                                    "assign_particles", VERBOSE);
    cl_kernel count_pp_collisions = getKernelWithUtils(device, context, PROJECT_DIR "/kernels/make_pp_collisions.cl",
                                                       "count_pp_collisions", VERBOSE);
    cl_kernel make_pp_collisions = getKernelWithUtils(device, context, PROJECT_DIR "/kernels/make_pp_collisions.cl",
                                                      "make_pp_collisions", VERBOSE);
    cl_command_queue queue = getCommandQueue(context, device, VERBOSE);


    // Generate control volumes and related arrays and buffers.
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
    gcv_pids = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong) * NUMPART, NULL, &ret);

    // Create particle-wall collisions (all possible collisions, i.e. no broad phase for particle-wall collisions).
    if (NUMWALLS > 0) {
        printf("[INIT] Creating particle-wall collisions\n");
        NUMPWCOLS = NUMPART * NUMWALLS;
        hpw_cols = malloc(sizeof(pw_collision) * NUMPWCOLS);
        for (cl_ulong p_id = 0; p_id < NUMPART; p_id++) {
            for (cl_ulong w_id = 0; w_id < NUMWALLS; w_id++) {
                cl_ulong id = w_id + p_id * NUMWALLS;
                hpw_cols[id].p_id = p_id;
                hpw_cols[id].w_id = w_id;
            }
        }

        gpw_cols = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(pw_collision) * NUMPWCOLS, NULL, &ret);
        ret = pw_collisionsToDevice(queue, gpw_cols, &hpw_cols, NUMPWCOLS);

        gwalls = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(aa_wall) * NUMWALLS, NULL, &ret);
        aa_wallsToDevice(queue, gwalls, &walls, NUMWALLS);
    }

    // Create gcollision_count buffer.
    gcollision_count = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_ulong), NULL, &ret);

    // Create gparticles buffer and copy particles into it.
    gparticles = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(particle) * NUMPART, NULL, &ret);
    ret = particlesToDevice(queue, gparticles, &hparticles, NUMPART);

    int int_periodic;  // Enumerated version of periodic boolean as booleans can't be passed to GPU by default.
    if (periodic) {
        int_periodic = 1;
    } else {
        int_periodic = 0;
    }

    // Set all kernel arguments. Note that if the cl_mem object changes they must be reset (e.g. gpp_cols).
    printf("[INIT] Setting kernel arguments\n");
    ret = clSetKernelArg(iterate_particle, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(iterate_particle, 1, sizeof(cl_float), &timestep);
    ret = clSetKernelArg(iterate_particle, 2, sizeof(cl_int), &int_periodic);
    ret = clSetKernelArg(iterate_particle, 3, sizeof(cl_float), &domain_length);

    ret = clSetKernelArg(calculate_pp_collision, 0, sizeof(cl_mem), &gpp_cols);
    ret = clSetKernelArg(calculate_pp_collision, 1, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(calculate_pp_collision, 2, sizeof(cl_float), &timestep);
    ret = clSetKernelArg(calculate_pp_collision, 3, sizeof(cl_float), &stiffness);
    ret = clSetKernelArg(calculate_pp_collision, 4, sizeof(cl_float), &restitution_coefficient);
    ret = clSetKernelArg(calculate_pp_collision, 5, sizeof(cl_float), &friction_coefficient);
    ret = clSetKernelArg(calculate_pp_collision, 6, sizeof(cl_float), &friction_stiffness);
    ret = clSetKernelArg(calculate_pp_collision, 7, sizeof(cl_float), &cohesion_stiffness);
    ret = clSetKernelArg(calculate_pp_collision, 8, sizeof(cl_float), &domain_length);

    ret = clSetKernelArg(assign_particle_count, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(assign_particle_count, 1, sizeof(cl_mem), &gparticle_count_array);
    ret = clSetKernelArg(assign_particle_count, 2, sizeof(cl_float), &domain_length);
    ret = clSetKernelArg(assign_particle_count, 3, sizeof(cl_float), &cv_length);
    ret = clSetKernelArg(assign_particle_count, 4, sizeof(cl_int), &cvs_per_edge);

    ret = clSetKernelArg(assign_particles, 0, sizeof(cl_mem), &gparticles);
    ret = clSetKernelArg(assign_particles, 1, sizeof(cl_mem), &gcv_start_array);
    ret = clSetKernelArg(assign_particles, 2, sizeof(cl_mem), &ginput_count_array);
    ret = clSetKernelArg(assign_particles, 3, sizeof(cl_mem), &gcv_pids);

    ret = clSetKernelArg(count_pp_collisions, 0, sizeof(cl_mem), &gparticle_count_array);
    ret = clSetKernelArg(count_pp_collisions, 1, sizeof(cl_int), &cvs_per_edge);
    ret = clSetKernelArg(count_pp_collisions, 2, sizeof(cl_mem), &gcollision_count);
    ret = clSetKernelArg(count_pp_collisions, 3, sizeof(cl_int), &int_periodic);

    ret = clSetKernelArg(make_pp_collisions, 0, sizeof(cl_mem), &gcv_start_array);
    ret = clSetKernelArg(make_pp_collisions, 1, sizeof(cl_mem), &gparticle_count_array);
    ret = clSetKernelArg(make_pp_collisions, 2, sizeof(cl_mem), &gcv_pids);
    ret = clSetKernelArg(make_pp_collisions, 3, sizeof(cl_int), &cvs_per_edge);
    ret = clSetKernelArg(make_pp_collisions, 4, sizeof(cl_mem), &gpp_cols);
    ret = clSetKernelArg(make_pp_collisions, 5, sizeof(cl_mem), &gcollision_count);
    ret = clSetKernelArg(make_pp_collisions, 6, sizeof(cl_int), &int_periodic);

    if (NUMWALLS > 0) {
        clSetKernelArg(calculate_pw_collision, 0, sizeof(cl_mem), &gpw_cols);
        clSetKernelArg(calculate_pw_collision, 1, sizeof(cl_mem), &gparticles);
        clSetKernelArg(calculate_pw_collision, 2, sizeof(cl_float), &timestep);
        clSetKernelArg(calculate_pw_collision, 3, sizeof(cl_mem), &gwalls);
        clSetKernelArg(calculate_pw_collision, 4, sizeof(cl_float), &stiffness);
        clSetKernelArg(calculate_pw_collision, 5, sizeof(cl_float), &restitution_coefficient);
        clSetKernelArg(calculate_pw_collision, 6, sizeof(cl_float), &friction_coefficient);
        clSetKernelArg(calculate_pw_collision, 7, sizeof(cl_float), &friction_stiffness);
    }

    // Do pre-simulation output and logging.
    printf("\nRunning sim with %llu particles, timestep %f, and log step %f, length %f.\n", NUMPART, timestep, log_step,
           sim_length);

    if (!writeTime(prefix, log_dir, NUMPART, "Start")) {
        return 1;
    }
    if (LOG_DATA) {
        printf("Logging at time: 0.000000\n");
        if (!writeParticles(hparticles, 0, prefix, log_dir, NUMPART, log_vel)) {
            return 1;
        }
    }

    cl_float last_write = 0; // Variable to store when logging was last run.

    // Run simulation.
    for (cl_float time = timestep; time <= sim_length; time += timestep) {
        if (VERBOSE) printf("Time = %f\n", time);

        // Count how many particles are in each CV.
        if (VERBOSE) printf("    Counting particles per CV\n");
        memset(particle_count_array, 0, sizeof(cl_int) * NUMCVS); // Reset counts to 0.
        ret = intArrayToDevice(queue, gparticle_count_array, &particle_count_array, NUMCVS);
        ret = clEnqueueNDRangeKernel(queue, assign_particle_count, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);

        // Set CV array starts according to the counts and assign particles to the cv_pid array.
        ret = intArrayToHost(queue, gparticle_count_array, &particle_count_array, NUMCVS);
        set_array_starts(particle_count_array, cv_start_array, NUMCVS);
        ulongArrayToDevice(queue, gcv_start_array, &cv_start_array, NUMCVS);
        memset(input_count_array, 0, sizeof(cl_int) * NUMCVS); // Reset counts to 0.
        intArrayToDevice(queue, ginput_count_array, &input_count_array, NUMCVS);
        if (VERBOSE) printf("    Assigning particles to CVs\n");
        ret = clEnqueueNDRangeKernel(queue, assign_particles, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);

        // Count collisions.
        if (VERBOSE) printf("    Counting collisions\n");
        collision_count = 0;
        clEnqueueWriteBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL, NULL);
        ret = clEnqueueNDRangeKernel(queue, count_pp_collisions, 1, NULL, &NUMCVS, 0, NULL, NULL, NULL);
        ret = clEnqueueReadBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL,
                                  NULL);
        if (ret != 0) {
            fprintf(stderr, "Error: Failed to read from collision count buffer. Error code %i\n", ret);
            return 1;
        }
        NUMPPCOLS = collision_count;

        if (NUMPPCOLS > 0) {
            // Make collisions.
            if (VERBOSE) printf("    Making %llu collisions\n", collision_count);
            if (gpp_cols != 0) {
                ret = clReleaseMemObject(gpp_cols);
            }
            gpp_cols = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(pp_collision) * NUMPPCOLS, NULL, &ret);
            if (ret != 0) {
                fprintf(stderr, "Error: Failed to create collision buffer. Error code %i\n", ret);
                return 1;
            }

            collision_count = 0;
            clEnqueueWriteBuffer(queue, gcollision_count, CL_TRUE, 0, sizeof(cl_ulong), &collision_count, 0, NULL,
                                 NULL);

            clSetKernelArg(make_pp_collisions, 4, sizeof(cl_mem), &gpp_cols);

            clEnqueueNDRangeKernel(queue, make_pp_collisions, 1, NULL, &NUMCVS, 0, NULL, NULL, NULL);

            // Calculate collisions.
            if (VERBOSE) printf("    Calculating collisions\n");
            ret = clSetKernelArg(calculate_pp_collision, 0, sizeof(cl_mem), &gpp_cols);
            ret = clEnqueueNDRangeKernel(queue, calculate_pp_collision, 1, NULL, &NUMPPCOLS, 0, NULL, NULL, NULL);
        }

        if (NUMWALLS > 0) {
            ret = clEnqueueNDRangeKernel(queue, calculate_pw_collision, 1, NULL, &NUMPWCOLS, 0, NULL, NULL, NULL);
        }

        // Iterate particles.
        if (VERBOSE) printf("    Iterating particles\n");
        ret = clEnqueueNDRangeKernel(queue, iterate_particle, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);
        if (LOG_DATA && time - last_write >= log_step) {
            ret = particlesToHost(queue, gparticles, &hparticles, NUMPART);
            printf("Logging at time: %f\n", time);
            if (!writeParticles(hparticles, time, prefix, log_dir, NUMPART, log_vel)) {
                return 1;
            }

            if (log_col_stats) {
                if (!writeCVStats(prefix, log_dir, NUMPART, particle_count_array, NUMCVS, time)) {
                    return 1;
                }
                if (!writeNumCols(prefix, log_dir, NUMPPCOLS, NUMPART, time)) {
                    return 1;
                }
            }

            last_write = time;
        }
    }

    if (LOG_DATA) {
        printf("Logging at time: %f\n", sim_length);
        if (!writeParticles(hparticles, sim_length, prefix, log_dir, NUMPART, log_vel)) {
            return 1;
        }
    }

    if (!writeTime(prefix, log_dir, NUMPART, "End")) {
        return 1;
    }
    return 0;
}