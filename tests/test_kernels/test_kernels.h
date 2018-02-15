#pragma warning (disable : 4996)

// Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../util/clUtils/clUtils.h"

boolean test_kernels(cl_device_id device, cl_context context, boolean verbose);