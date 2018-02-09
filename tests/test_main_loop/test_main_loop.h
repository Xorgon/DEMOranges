//
// Created by Elijah on 08/02/2018.
//

#ifndef DEMORANGES_TEST_MAIN_LOOP_H
#define DEMORANGES_TEST_MAIN_LOOP_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../util/clUtils.h"
#include "../../util/cvUtils.h"
#include "../../structures/particle.h"

boolean test_main_loop(cl_device_id device, cl_context context, boolean verbose);

#endif //DEMORANGES_TEST_MAIN_LOOP_H
