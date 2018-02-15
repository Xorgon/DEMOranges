//
// Created by Elijah on 05/12/2017.
//

#ifndef DEMORANGES_CLDEVICEUTILS_H
#define DEMORANGES_CLDEVICEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "../../structures/particle.h"
#include "../../structures/collision.h"
#include "../../structures/wall.h"
#include <stdbool.h>

#define MAX_SOURCE_SIZE (0x100000)

// If boolean is not correctly defined.
#if !defined(boolean)
#define boolean bool
#define TRUE true
#define FALSE false
#endif

void setContext(cl_device_id *device, cl_context *context,
                boolean verbose);

void printDeviceDetails(cl_uint platformCount, cl_platform_id *platforms, cl_uint deviceCount, cl_device_id *devices);

cl_command_queue getCommandQueue(cl_context context, cl_device_id device, boolean verbose);

cl_context getContext(cl_device_id **devices, cl_uint num_devices, boolean verbose);

cl_kernel getKernel(cl_device_id device, cl_context context, char fileName[], char kernelName[], boolean verbose);

cl_int particlesToDevice(cl_command_queue queue, cl_mem gparticles, particle **hparticles, cl_ulong NUMPART);

cl_int particlesToHost(cl_command_queue queue, cl_mem gparticles, particle **hparticles, cl_ulong NUMPART);

cl_int
pp_collisionsToDevice(cl_command_queue queue, cl_mem gpp_collisions, pp_collision **hpp_collisions, cl_ulong NUMCOLS);

cl_int
pp_collisionsToHost(cl_command_queue queue, cl_mem gpp_collisions, pp_collision **hpp_collisions, cl_ulong NUMCOLS);

cl_int
pw_collisionsToDevice(cl_command_queue queue, cl_mem gpw_collisions, pw_collision **hpw_collisions, cl_ulong NUMCOLS);

cl_int
pw_collisionsToHost(cl_command_queue queue, cl_mem gpw_collisions, pw_collision **hpw_collisions, cl_ulong NUMCOLS);

cl_int aa_wallsToDevice(cl_command_queue queue, cl_mem gaa_walls, aa_wall **haa_walls, cl_ulong NUMWALLS);

cl_int aa_wallsToHost(cl_command_queue queue, cl_mem gaa_walls, aa_wall **haa_walls, cl_ulong NUMWALLS);

cl_int intArrayToDevice(cl_command_queue queue, cl_mem array_buffer,
                        cl_int **array, cl_ulong length);

cl_int intArrayToHost(cl_command_queue queue, cl_mem array_buffer, cl_int **array,
                      cl_ulong length);

cl_int ulongArrayToDevice(cl_command_queue queue, cl_mem array_buffer,
                          cl_ulong **array, cl_ulong length);

cl_int ulongArrayToHost(cl_command_queue queue, cl_mem array_buffer, cl_ulong **array,
                        cl_ulong length);

#endif //DEMORANGES_CLDEVICEUTILS_H
