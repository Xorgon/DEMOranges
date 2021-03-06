//
// Created by Elijah on 20/12/2017.
//

#ifndef DEMORANGES_TEST_ATOMICS_H
#define DEMORANGES_TEST_ATOMICS_H

#include <malloc.h>
#include "CL/cl.h"
#include "../../util/clUtils/clUtils.h"

boolean test_atomics(cl_device_id device, cl_context context, boolean verbose);

#endif //DEMORANGES_TEST_ATOMICS_H
