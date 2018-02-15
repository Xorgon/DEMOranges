//
// Created by Elijah on 31/01/2018.
//

#ifndef DEMORANGES_TEST_ASSIGN_PARTICLES_H
#define DEMORANGES_TEST_ASSIGN_PARTICLES_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../util/clUtils/clUtils.h"
#include "../../util/cvUtils/cvUtils.h"
#include "../../structures/particle.h"

boolean test_assign_particle_count(cl_device_id device, cl_context context, boolean verbose);

boolean test_set_array_starts(boolean verbose);

boolean test_assign_particles(cl_device_id device, cl_context context, boolean verbose);


#endif //DEMORANGES_TEST_ASSIGN_PARTICLES_H
