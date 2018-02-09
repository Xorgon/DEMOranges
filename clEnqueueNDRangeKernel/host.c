#pragma warning (disable : 4996)

// Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <math.h>
#include <sys\timeb.h>
#include <conio.h>
#include "../util/clUtils.h"

// Defining variables
#define MEM_SIZE (128)
#define MATH_PI 3.14159265358979323846

// Device and platform info variables

cl_device_id device;
cl_context context;

// Debugging strings
cl_int ret;
char string[MEM_SIZE];

// Simulation parameters
float rho = 2000;               // particle density
float D = 0.001;                          // particle diameter
float mu = 0.0000193;                    // fluid viscosity
float g = -0.001;                      // gravitational acceleration
float flowMag = 0.14;                // maximum velocity of the flow field
float vortexFreq = 1;                    // scalar to define the vortex density
float tauMultiplier = 1;                // scalar to change the tau value
float timeDurationFactor = 99;  // defines length of simulation

int NUMPART = 1000000;          // number of particles
int LOCAL_SIZE = 1;             // local work size
int log_step = 50;              // log files for intermediate particle position and velocities written every 2*log_step

// GPU and host buffers for particle position and velocity
float *buffer;
float *hpos;
float *hvel;
cl_mem gposold;
cl_mem gvelold;
cl_mem gposnew;
cl_mem gvelnew;

// GPU and host buffers for simulation variables
float *h_tau;
float *h_tstep;
cl_mem g_tau;
cl_mem g_tstep;


// PROGRAM BEGINS HERE
int main() {

    // Initializing OpenCL.
    setContext(&device, &context, TRUE);
    cl_kernel kernel = getKernel(device, context, "../clEnqueueNDRangeKernel/propagate.cl", "propagate", TRUE);
    cl_command_queue queue = getCommandQueue(context, device, TRUE);

    // Create memory buffers
    gposold = clCreateBuffer(context, CL_MEM_READ_WRITE, NUMPART * sizeof(cl_float4), NULL, &ret);
    gvelold = clCreateBuffer(context, CL_MEM_READ_WRITE, NUMPART * sizeof(cl_float4), NULL, &ret);
    gposnew = clCreateBuffer(context, CL_MEM_READ_WRITE, NUMPART * sizeof(cl_float4), NULL, &ret);
    gvelnew = clCreateBuffer(context, CL_MEM_READ_WRITE, NUMPART * sizeof(cl_float4), NULL, &ret);
    printf("[INIT] Create memory buffers: ");
    if ((int) ret == 0) {
        printf("SUCCESS\n");
    } else {
        printf("FAILED (%d)\n", ret);
        _getch();
        return 1;
    }

    // Calculate the TGV constants
    float A = flowMag;
    float a = vortexFreq;
    float B = A;
    float b = a;
    float C = -2 * A;
    float c = a;
    printf("[ SIM] Defining TGV values: SUCCESS\n");

    // Define characteristic values of simulation
    float tau = tauMultiplier * (rho * D * D) / (18 * mu);
    float tstep = 0.1 * tau;
    float vT = -g * tau;
    float finalT = timeDurationFactor * tau;
    printf("[ SIM] Defining characteristic values of simulation: SUCCESS\n\n"
                   "             Tau value: %6f\n"
                   "             Time step: %6f\n"
                   "             Terminal velocity: %6f\n"
                   "             Final time: %6f\n\n", tau, tstep, vT, finalT);

    // Generate random initial values for position
    srand(0);
    hpos = malloc(sizeof(float) * 4 * NUMPART);
    for (int ip = 0; ip < NUMPART; ip++) {
        hpos[ip * 4 + 0] = 3.1 * (float) ((float) rand() / (float) RAND_MAX) - 1.6;
        hpos[ip * 4 + 1] = 3.1 * (float) ((float) rand() / (float) RAND_MAX) - 1.6;
        hpos[ip * 4 + 2] = 3.1 * (float) ((float) rand() / (float) RAND_MAX) - 1.6;
        hpos[ip * 4 + 3] = 0.0f;
    }
    printf("[ SIM] Create %d initial position values: SUCCESS\n", NUMPART);

    // Define initial particle velocities based on local TGV velocities
    hvel = malloc(sizeof(float) * 4 * NUMPART);
    for (int ip = 0; ip < NUMPART; ip++) {
        hvel[ip * 4 + 0] = A * cos(a * ((float) hpos[ip * 4 + 0] + ((float) MATH_PI / (a * 2)))) *
                           sin(b * ((float) hpos[ip * 4 + 1] + ((float) MATH_PI / (2 * a)))) *
                           sin(c * ((float) hpos[ip * 4 + 2] + ((float) MATH_PI / (2 * a))));
        hvel[ip * 4 + 1] = B * sin(a * ((float) hpos[ip * 4 + 0] + ((float) MATH_PI / (a * 2)))) *
                           cos(b * ((float) hpos[ip * 4 + 1] + ((float) MATH_PI / (2 * a)))) *
                           sin(c * ((float) hpos[ip * 4 + 2] + ((float) MATH_PI / (2 * a))));
        hvel[ip * 4 + 2] = C * sin(a * ((float) hpos[ip * 4 + 0] + ((float) MATH_PI / (a * 2)))) *
                           sin(b * ((float) hpos[ip * 4 + 1] + ((float) MATH_PI / (2 * a)))) *
                           cos(c * ((float) hpos[ip * 4 + 2] + ((float) MATH_PI / (2 * a))));
        hvel[ip * 4 + 3] = 0.0f;
    }
    printf("[ SIM] Create %d initial velocity values: SUCCESS\n", NUMPART);
    printf("  POSITION: %6f %6f %6f %6f\n", hpos[0], hpos[1], hpos[2], hpos[3]);
    printf("  VELOCITY: %6f %6f %6f %6f\n", hvel[0], hvel[1], hvel[2], hvel[3]);

    // Write initial values to file
    FILE *fd = fopen("INITIAL VALUES.txt", "w");
    for (int i = 0; i < NUMPART; i++) {
        fprintf(fd, "%e %e %e %e", hpos[i * 4 + 0], hpos[i * 4 + 1], hpos[i * 4 + 2], hpos[i * 4 + 3]);
        fprintf(fd, " %e %e %e %e\n", hvel[i * 4 + 0], hvel[i * 4 + 1], hvel[i * 4 + 2], hvel[i * 4 + 3]);
    }
    fclose(fd);

    // pass relevant values to memory buffer on the GPU
    ret = clEnqueueWriteBuffer(queue, gposold, CL_TRUE, 0, NUMPART * sizeof(cl_float4), hpos, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(queue, gvelold, CL_TRUE, 0, NUMPART * sizeof(cl_float4), hvel, 0, NULL, NULL);
    printf("[ SIM] Copying values to GPU memory buffers: ");
    if ((int) ret == 0) {
        printf("SUCCESS\n");
    } else {
        printf("FAILED (%d)\n", ret);
        _getch();
        return 1;
    }

    // set OpenCL kernel parameters
    ret = clSetKernelArg(kernel, 4, sizeof(float), &tau);
    ret = clSetKernelArg(kernel, 5, sizeof(float), &A);
    ret = clSetKernelArg(kernel, 6, sizeof(float), &a);
    ret = clSetKernelArg(kernel, 7, sizeof(float), &B);
    ret = clSetKernelArg(kernel, 8, sizeof(float), &b);
    ret = clSetKernelArg(kernel, 9, sizeof(float), &C);
    ret = clSetKernelArg(kernel, 10, sizeof(float), &c);
    ret = clSetKernelArg(kernel, 11, sizeof(float), &tstep);
    printf("[ SIM] Set OpenCL kernel parameters: ");
    if ((int) ret == 0) {
        printf("SUCCESS\n");
    } else {
        printf("FAILED (%d)\n", ret);
        _getch();
        return 1;
    }\

    // SIMULATION BEGINS HERE
    int count = 0;
    int *particle;

    printf("\nPress any key to start simulation.\n\n");
    _getch();

    // start logging simulation time
    struct timeb start, end;
    int diff;
    int i = 0;
    ftime(&start);
    double writing = 0;

    for (float time = 0; time < finalT; time += 2 * tstep) {
        // If an intermediate log file is required
        if (count % log_step == 0) {
            // copy results from memory buffer
            ret = clEnqueueReadBuffer(queue, gposold, CL_TRUE, 0, NUMPART * sizeof(cl_float4), hpos, 0, NULL, NULL);
            ret = clEnqueueReadBuffer(queue, gvelold, CL_TRUE, 0, NUMPART * sizeof(cl_float4), hvel, 0, NULL, NULL);
            printf("[ SIM] Finished round of particles for timestep: %3f / %3f\n, writing intermediate results for this step to file.",
                   time, finalT);
            printf("  POSITION: %6f %6f %6f %6f\n", hpos[0], hpos[1], hpos[2], hpos[3]);
            printf("  VELOCITY: %6f %6f %6f %6f\n", hvel[0], hvel[1], hvel[2], hvel[3]);

            struct timeb write_start, write_end;
            ftime(&write_start);
            // write intermediate results to file
            char filename[500];
            sprintf(filename, "SIM_RUN_%d.txt", count * 2);
            fd = fopen(filename, "w");
            for (int i = 0; i < NUMPART; i++) {
                fprintf(fd, "%e %e %e %e ", hpos[i * 4 + 0], hpos[i * 4 + 1], hpos[i * 4 + 2], hpos[i * 4 + 3]);
                fprintf(fd, "%e %e %e %e\n", hvel[i * 4 + 0], hvel[i * 4 + 1], hvel[i * 4 + 2], hvel[i * 4 + 3]);
            }
            fclose(fd);
            ftime(&write_end);
            writing += write_end.time - write_start.time;

        }
        // Reverse the kernel arguments
        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gposold);
        ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &gvelold);
        ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &gposnew);
        ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &gvelnew);
        // Execute kernel over NDRange
        ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);
        // Wait for this step to finish
        clFinish(queue);

        // Reverse the kernel arguments (new > old; old > new)
        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gposnew);
        ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &gvelnew);
        ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &gposold);
        ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &gvelold);
        // Execute kernel over NDRange
        ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &NUMPART, 0, NULL, NULL, NULL);
        // Wait for this step to finish
        clFinish(queue);
        count++;
    }

    // Log simulation time
    ftime(&end);
    diff = (int) (1000.0 * (end.time - start.time)
                  + (end.millitm - start.millitm));

    printf("\nOperation took %f seconds\n", diff / 1000.);
    printf("\nOf which writing was %f seconds (%f%%)\n", writing, 100 * writing / (diff / 1000.));

    getch();
}
