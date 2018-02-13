//
// Created by Elijah on 05/12/2017.
//

#include "clUtils.h"

void setContext(cl_device_id *device, cl_context *context,
                boolean verbose) {
    cl_platform_id *platforms;
    cl_device_id *devices;
    cl_uint platformCount;
    cl_uint deviceCount;

    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id *) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);

    for (cl_uint i = 0; i < platformCount; i++) {
        // get all devices
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
        devices = (cl_device_id *) malloc(sizeof(cl_device_id) * deviceCount);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);
    }

    if (verbose) {
        printDeviceDetails(platformCount, platforms, deviceCount, devices);
    }

    // select OpenCL platform and device
    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id *) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);

    int platform_id = 0;
    if (verbose && platformCount > 1) {
        printf("Enter platform number:\n");
        boolean valid = FALSE;
        while (!valid) {
            int input;
            if (scanf("%d", &input) > 0 && input < platformCount && input >= 0) {
                platform_id = input;
                valid = TRUE;
            } else {
                printf("Invalid input, select a platform number no higher than %i\n", platformCount - 1);
                getchar();
            }
        }
    }

    // Change the first argument of clGetDeviceIDs to the desired platform from initial system diagnosis, default is set to platforms[0]
    clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
    devices = (cl_device_id *) malloc(sizeof(cl_device_id) * deviceCount);
    clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

    int device_id = 0;
    if (verbose && deviceCount > 1) {
        printf("Enter device number:\n");
        boolean valid = FALSE;
        while (!valid) {
            int input;
            if (scanf("%d", &input) > 0 && input < deviceCount && input >= 0) {
                device_id = input;
                valid = TRUE;
            } else {
                printf("Invalid input, select a device number no higher than %i\n", deviceCount - 1);
                getchar();
            }
        }
    }

    *device = devices[device_id];

    char *value;
    size_t valueSize;

    clGetDeviceInfo(devices[device_id], CL_DEVICE_NAME, 0, NULL, &valueSize);
    value = (char *) malloc(valueSize);
    clGetDeviceInfo(devices[device_id], CL_DEVICE_NAME, valueSize, value, NULL);
    if (verbose) printf("Selected device: %s\n", value);
    free(value);

    devices[0] = *device; // Only send selected device.
    *context = getContext(&devices, 1, verbose);
}

void printDeviceDetails(cl_uint platformCount, cl_platform_id *platforms, cl_uint deviceCount, cl_device_id *devices) {

    cl_uint maxComputeUnits;
    char *value;
    size_t valueSize;

    if (platformCount == 0) {
        printf("No OpenCL platforms available!\n");
    }

    for (cl_uint i = 0; i < platformCount; i++) {
        printf("--------------\n");
        printf(" PLATFORM %d \n", i);
        printf("--------------\n");
        // for each device print critical attributes
        for (cl_uint j = 0; j < deviceCount; j++) {
            // print device name
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
            value = (char *) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
            printf("%d. Device: %s\n", j, value);
            free(value);

            // print hardware device version
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
            value = (char *) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
            printf("   %d.%d Hardware version: %s\n", j, 1, value);
            free(value);

            // print software driver version
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
            value = (char *) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
            printf("   %d.%d Software version: %s\n", j, 2, value);
            free(value);

            // print c version supported by compiler for device
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
            value = (char *) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
            printf("   %d.%d OpenCL C version: %s\n", j, 3, value);
            free(value);

            // device type
            cl_device_type devtype;
            clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(devtype), &devtype, NULL);
            if (devtype == CL_DEVICE_TYPE_CPU) printf("   %d.%d Device type: CPU\n", j, 4);
            else if (devtype == CL_DEVICE_TYPE_GPU) printf("   %d.%d Device type: GPU\n", j, 4);
            else if (devtype == CL_DEVICE_TYPE_ACCELERATOR) printf("   %d.%d Device type: ACCELERATOR\n", j, 4);
            else printf("   %d.%d Device type: UNKNOWN\n", j, 4);

            // print parallel compute units
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxComputeUnits), &maxComputeUnits, NULL);
            printf("   %d.%d Parallel compute units: %d\n", j, 5, maxComputeUnits);

            // max work group size
            size_t devcores2;
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(devcores2), &devcores2, NULL);
            printf("   %d.%d Max work group size: %u\n", j, 6, (cl_uint) devcores2);

            // max work item size
            size_t work_item_size[3] = {0, 0, 0};
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(work_item_size), &work_item_size, NULL);
            printf("   %d.%d Max work items: (%d, %d, %d)\n", j, 7, work_item_size[0], work_item_size[1],
                   work_item_size[2]);

            // max clock frequency
            cl_uint devfreq;
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(devfreq), &devfreq, NULL);
            printf("   %d.%d Max clock frequency: %u\n", j, 8, devfreq);

            // memory in MB
            cl_ulong devmem;
            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(devmem), &devmem, NULL);
            cl_uint devmemMB = (cl_uint) (devmem / 1000000);
            printf("   %d.%d Device global memory (MB): %u\n", j, 9, devmemMB);

            // double precision support?
            cl_int supported;
            clGetDeviceInfo(devices[j], CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, sizeof(supported), &supported, NULL);
            printf("   %d.%d Double precision supported: %s\n\n", j, 10, supported ? "YES" : "NO");

        }
        free(devices);
    }
    free(platforms);
}

cl_context getContext(cl_device_id **devices, cl_uint num_devices, boolean verbose) {
    // Create OpenCL context
    cl_int ret;

    cl_context context = NULL;
    context = clCreateContext(NULL, num_devices, *devices, NULL, NULL, &ret);
    if (verbose) printf("[INIT] Create OpenCL context: ");
    if ((int) ret == 0) {
        if (verbose) printf("SUCCESS\n");
    } else {
        if (verbose) {
            printf("FAILED (%i)\n", ret);
            getchar();
        }
        return NULL;
    }
    return context;
}

cl_command_queue getCommandQueue(cl_context context, cl_device_id device, boolean verbose) {
    // Create command queue
    cl_int ret;

    cl_command_queue queue = NULL;
    queue = clCreateCommandQueue(context, device, 0, &ret);
    if (verbose) printf("[INIT] Create command queue: ");
    if ((int) ret == 0) {
        if (verbose) printf("SUCCESS\n");
    } else {
        fprintf(stderr, "Failed to create command queue. Error %i\n", ret);
        if (verbose) {
            printf("FAILED\n");
            getchar();
        }
        return NULL;
    }
    return queue;
}

//TODO: Add boolean to only optionally include utils and make util includes work.
cl_kernel getKernel(cl_device_id device, cl_context context, char fileName[], char kernelName[], boolean verbose) {
    FILE *fp;
    char *source_str;
    size_t source_size;

    char *utils_str;
    size_t utils_size;
    cl_int ret;

    if (verbose) printf("[INIT] Creating %s kernel\n", kernelName);
    // load source code containing utilities.
    fp = fopen("../util/kernelUtils.cl", "r");
//    fopen_s(&fp, "../util/kernelUtils.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load util file.\n");
        exit(1);
    }
    utils_str = (char *) malloc(MAX_SOURCE_SIZE);
    utils_size = fread(utils_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    // load source code containing kernel
    fp = fopen(fileName, "r");
//    fopen_s(&fp, fileName, "r");
    if (!fp) {
        if (verbose) fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char *) malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);
    const char *strings[] = {utils_str, source_str};
    const size_t sizes[] = {utils_size, source_size};
    // Create kernel program from source
    cl_program program = NULL;
    program = clCreateProgramWithSource(context, 2, strings, sizes, &ret);
    if (verbose) printf("       Create kernel program: ");
    if ((int) ret == 0) {
        if (verbose) printf("SUCCESS\n");
    } else {
        if (verbose) {
            printf("FAILED (%d)\n", ret);
            getchar();
        }
        return NULL;
    }

    cl_device_id devices[] = {device};
    // Build kernel program
    ret = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
    if (verbose) printf("       Build kernel program: ");
    if ((int) ret == 0) {
        if (verbose) printf("SUCCESS\n");
    } else {
        if (verbose) printf("FAILED (%d)\n", ret);
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        if (verbose) {
            printf("%s\n", log);
            getchar();
        }
        return NULL;
    }

    // Create OpenCL kernel
    cl_kernel kernel = NULL;
    kernel = clCreateKernel(program, kernelName, &ret);
    if (verbose) printf("       Create OpenCL kernel: ");
    if ((int) ret == 0) {
        if (verbose) printf("SUCCESS\n");
    } else {
        if (verbose) {
            printf("FAILED (%d)\n", ret);
            getchar();
        }
        return NULL;
    }
    return kernel;
}

cl_int particlesToDevice(cl_command_queue queue, cl_mem gparticles, particle **hparticles, cl_ulong NUMPART) {
    return clEnqueueWriteBuffer(queue, gparticles, CL_TRUE, 0, sizeof(particle) * NUMPART, *hparticles, 0, NULL, NULL);
}

cl_int particlesToHost(cl_command_queue queue, cl_mem gparticles, particle **hparticles, cl_ulong NUMPART) {
    return clEnqueueReadBuffer(queue, gparticles, CL_TRUE, 0, sizeof(particle) * NUMPART, *hparticles, 0, NULL, NULL);
}

cl_int
pp_collisionsToDevice(cl_command_queue queue, cl_mem gpp_collisions, pp_collision **hpp_collisions, cl_ulong NUMCOLS) {
    return clEnqueueWriteBuffer(queue, gpp_collisions, CL_TRUE, 0, sizeof(pp_collision) * NUMCOLS, *hpp_collisions, 0,
                                NULL, NULL);
}

cl_int
pp_collisionsToHost(cl_command_queue queue, cl_mem gpp_collisions, pp_collision **hpp_collisions, cl_ulong NUMCOLS) {
    return clEnqueueReadBuffer(queue, gpp_collisions, CL_TRUE, 0, sizeof(pp_collision) * NUMCOLS, *hpp_collisions, 0,
                               NULL, NULL);
}

cl_int
pw_collisionsToDevice(cl_command_queue queue, cl_mem gpw_collisions, pw_collision **hpw_collisions, cl_ulong NUMCOLS) {
    return clEnqueueWriteBuffer(queue, gpw_collisions, CL_TRUE, 0, sizeof(pw_collision) * NUMCOLS, *hpw_collisions, 0,
                                NULL, NULL);
}

cl_int
pw_collisionsToHost(cl_command_queue queue, cl_mem gpw_collisions, pw_collision **hpw_collisions, cl_ulong NUMCOLS) {
    return clEnqueueReadBuffer(queue, gpw_collisions, CL_TRUE, 0, sizeof(pw_collision) * NUMCOLS, *hpw_collisions, 0,
                               NULL, NULL);
}

cl_int intArrayToDevice(cl_command_queue queue, cl_mem array_buffer, cl_int **array,
                        cl_ulong length) {
    return clEnqueueWriteBuffer(queue, array_buffer, CL_TRUE, 0, sizeof(cl_int) * length,
                                *array, 0, NULL, NULL);
}

cl_int intArrayToHost(cl_command_queue queue, cl_mem array_buffer, cl_int **array,
                      cl_ulong length) {
    return clEnqueueReadBuffer(queue, array_buffer, CL_TRUE, 0, sizeof(cl_int) * length, *array,
                               0, NULL, NULL);
}

cl_int ulongArrayToDevice(cl_command_queue queue, cl_mem array_buffer, cl_ulong **array,
                          cl_ulong length) {
    return clEnqueueWriteBuffer(queue, array_buffer, CL_TRUE, 0, sizeof(cl_ulong) * length,
                                *array, 0, NULL, NULL);
}

cl_int ulongArrayToHost(cl_command_queue queue, cl_mem array_buffer, cl_ulong **array,
                        cl_ulong length) {
    return clEnqueueReadBuffer(queue, array_buffer, CL_TRUE, 0, sizeof(cl_ulong) * length, *array,
                               0, NULL, NULL);
}