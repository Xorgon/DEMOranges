//
// Created by Elijah on 05/12/2017.
//

#ifndef DEMORANGES_CLDEVICEUTILS_H
#define DEMORANGES_CLDEVICEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <math.h>
#include <sys\timeb.h>
#include <conio.h>

void printDeviceDetails(cl_uint *platformCount, cl_platform_id *id, cl_uint *deviceCount, cl_device_id *devices);

#endif //DEMORANGES_CLDEVICEUTILS_H
