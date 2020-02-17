//
// Created by Elijah on 03/03/2018.
//

#include "../../util/clUtils/clUtils.h"
#include "../../tests/run_tests/run_tests.h"
#include "../../util/simUtils/simUtils.h"
#include <CL/cl.h>
#include <malloc.h>
#include <string.h>

#ifndef DEMORANGES_SIMRUNNER_H
#define DEMORANGES_SIMRUNNER_H

int runSim(particle *hparticles, cl_ulong NUMPART, cl_kernel iterate_particle, cl_float particle_diameter,
           aa_wall *walls, cl_ulong NUMWALLS, cl_bool periodic,
           cl_float stiffness, cl_float wall_stiffness, cl_float restitution_coefficient, cl_float friction_coefficient,
           cl_float friction_stiffness, cl_float cohesion_stiffness,
           cl_float domain_length, char prefix[], char log_dir[], float sim_length, float timestep,
           boolean VERBOSE, boolean LOG_DATA, boolean log_vel, boolean log_col_stats, float log_step,
           cl_device_id device, cl_context context);

#endif //DEMORANGES_SIMRUNNER_H
