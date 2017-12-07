//
// Created by Elijah on 05/12/2017.
//

#ifndef DEMORANGES_CLDEVICEUTILS_H
#define DEMORANGES_CLDEVICEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

void setDevices(cl_platform_id **platforms, cl_device_id **devices);

void printDeviceDetails(cl_uint *platformCount, cl_platform_id *id, cl_uint *deviceCount, cl_device_id *devices);

cl_kernel getKernel(cl_device_id **devices, cl_context *context, char fileName[], char kernelName[]);

#endif //DEMORANGES_CLDEVICEUTILS_H
