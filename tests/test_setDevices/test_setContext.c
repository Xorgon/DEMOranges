//
// Created by Elijah on 09/02/2018.
//

#include "test_setContext.h"

boolean test_setContext(boolean verbose) {
    cl_device_id device;
    cl_context context;

    setContext(&device, &context, TRUE);

    return TRUE;
}