//
// Created by Elijah on 06/02/2018.
//

#ifndef DEMORANGES_TEST_MAKE_PP_COLLISIONS_H
#define DEMORANGES_TEST_MAKE_PP_COLLISIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../util/clUtils/clUtils.h"
#include "../../util/cvUtils/cvUtils.h"
#include "../../structures/particle.h"

boolean test_count_pp_collisions(cl_device_id device, cl_context context, boolean verbose);

boolean test_make_pp_collisions(cl_device_id device, cl_context context, boolean verbose);

#endif //DEMORANGES_TEST_MAKE_PP_COLLISIONS_H
