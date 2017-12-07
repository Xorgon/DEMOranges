#pragma warning (disable : 4996)

// Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <conio.h>
#include "../util/clUtils.h"

// Defining variables
#define MEM_SIZE (128)
#define MATH_PI 3.14159265358979323846

// Device and platform info variables

cl_platform_id *platforms;
cl_device_id *devices;


// Debugging strings
cl_int ret;
char string[MEM_SIZE];

// Source file variables
char *fileNames[] = {
        "../kernels/iterate_particle.cl",
//        "../kernels/make_pp_collisions.cl",
//        "../kernels/make_pw_collisions.cl",
        "../kernels/pp_collision.cl",
        "../kernels/pw_collision.cl",
//        "../kernels/sort_particles.cl"
};

char *kernelNames[] = {
        "iterate_particle",
//        "make_pp_collisions",
//        "make_pw_collisions",
        "pp_collision",
        "pw_collision",
//        "sort_particles"
};
int files = 3;

// PROGRAM BEGINS HERE
int main() {
    setDevices(&platforms, &devices);

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

    for (int i = 0; i < files; i++) {

        printf("\nKernel file: %s\n", fileNames[i]);
        printf("Kernel name: %s\n", kernelNames[i]);

        cl_kernel kernel = getKernel(&devices, &context, fileNames[i], kernelNames[i]);
    }
}