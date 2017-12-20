//
// Created by Elijah on 05/12/2017.
//

#ifndef DEMORANGES_CLDEVICEUTILS_H
#define DEMORANGES_CLDEVICEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../structures/particle.h"
#include "../structures/collision.h"

#define MAX_SOURCE_SIZE (0x100000)

void setDevices(cl_platform_id **platforms, cl_device_id **devices, boolean verbose);

void printDeviceDetails(cl_uint platformCount, cl_platform_id *platforms, cl_uint deviceCount, cl_device_id *devices);

cl_command_queue getCommandQueue(cl_context *context, cl_device_id **devices, boolean verbose);

cl_context getContext(cl_device_id **devices, boolean verbose);

cl_kernel getKernel(cl_device_id **devices, cl_context *context, char fileName[], char kernelName[], boolean verbose);

cl_int particlesToDevice(cl_command_queue queue, cl_mem gparticles, particle **hparticles, cl_ulong NUMPART);

cl_int particlesToHost(cl_command_queue queue, cl_mem gparticles, particle **hparticles, cl_ulong NUMPART);

cl_int
pp_collisionsToDevice(cl_command_queue queue, cl_mem gpp_collisions, pp_collision **hpp_collisions, cl_ulong NUMCOLS);

cl_int
pp_collisionsToHost(cl_command_queue queue, cl_mem gpp_collisions, pp_collision **hpp_collisions, cl_ulong NUMCOLS);

#endif //DEMORANGES_CLDEVICEUTILS_H
