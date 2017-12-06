#pragma warning (disable : 4996)

// Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <conio.h>

// Defining variables
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)
#define MATH_PI 3.14159265358979323846

// Device and platform info variables
char *value;
size_t valueSize;
cl_uint platformCount;
cl_platform_id *platforms;
cl_uint deviceCount;
cl_device_id *devices;


// Debugging strings
cl_int ret;
char string[MEM_SIZE];

// Source file variables
FILE *fp;
char *fileNames[] = {
        "../kernels/iterate_particle.cl",
//        "../kernels/make_pp_collisions.cl",
//        "../kernels/make_pw_collisions.cl",
        "../kernels/pp_collision.cl",
        "../kernels/pw_collision.cl",
//        "../kernels/sort_particles.cl"
};
int files = 3;
char *source_str;
size_t source_size;

// PROGRAM BEGINS HERE
int main() {
    for (int i = 0; i < files; i++) {

        printf("\nKernel file: %s\n", fileNames[i]);

        // load source code containing kernel
        fopen_s(&fp, fileNames[i], "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }
        source_str = (char *) malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);

        // select OpenCL platform and device
        clGetPlatformIDs(0, NULL, &platformCount);
        platforms = (cl_platform_id *) malloc(sizeof(cl_platform_id) * platformCount);
        clGetPlatformIDs(platformCount, platforms, NULL);

        // Change the first argument of clGetDeviceIDs to the desired platform from initial system diagnosis, default is set to platforms[0]
        clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
        devices = (cl_device_id *) malloc(sizeof(cl_device_id) * deviceCount);
        clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

        // Change the first argument of clGetDeviceInfo to the desired device from initial system diagnosis, default is set to devices[0]
        clGetDeviceInfo(devices[0], CL_DEVICE_NAME, 0, NULL, &valueSize);
        value = (char *) malloc(valueSize);
        clGetDeviceInfo(devices[0], CL_DEVICE_NAME, valueSize, value, NULL);
        printf("Default computing device selected: %s\n", value);
        free(value);

        // Create OpenCL context
        cl_context context = NULL;
        context = clCreateContext(NULL, 1, &devices[0], NULL, NULL, &ret);
        printf("[INIT] Create OpenCL context: ");
        if ((int) ret == 0) {
            printf("SUCCESS\n");
        } else {
            printf("FAILED\n");
            _getch();
            return 1;
        }

        // Create kernel program from source
        cl_program program = NULL;
        program = clCreateProgramWithSource(context, 1, (const char **) &source_str, (const size_t *) &source_size,
                                            &ret);
        printf("[INIT] Create kernel program: ");
        if ((int) ret == 0) {
            printf("SUCCESS\n");
        } else {
            printf("FAILED (%d)\n", ret);
            _getch();
            return 1;
        }

        // Build kernel program
        ret = clBuildProgram(program, 1, &devices[0], NULL, NULL, NULL);
        printf("[INIT] Build kernel program: ");
        if ((int) ret == 0) {
            printf("SUCCESS\n");
        } else {
            printf("FAILED (%d)\n", ret);
            // Determine the size of the log
            size_t log_size;
            clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
            // Allocate memory for the log
            char *log = (char *) malloc(log_size);

            // Get the log
            clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

            // Print the log
            printf("%s\n", log);
            _getch();
            return 1;
        }
    }
}