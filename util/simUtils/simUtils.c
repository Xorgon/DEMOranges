//
// Created by Elijah on 15/02/2018.
//

#include "simUtils.h"

void writeParticles(particle *particles, float time, char prefix[], char dir[], cl_ulong NUMPART, boolean log_vel) {
    char filename[500];
    sprintf(filename, "%s%llu_%s_%i.txt", dir, NUMPART, prefix, (long) roundf(time * 1e6));
    FILE *fd = fopen(filename, "w");
    for (int i = 0; i < NUMPART; i++) {
        if (log_vel) {
            fprintf(fd, "%f,%f,%f,%f,%f,%f\n", particles[i].pos.x, particles[i].pos.y, particles[i].pos.z,
                    particles[i].vel.x, particles[i].vel.y, particles[i].vel.z);
        } else {
            fprintf(fd, "%f,%f,%f\n", particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
        }
    }
    fclose(fd);
}

void writeSetupData(char prefix[], char dir[], cl_ulong NUMPART, cl_float timestep, cl_float sim_length,
                    cl_float domain_length, cl_float stiffness, cl_float restitution_coefficient,
                    cl_float friction_coefficient, cl_float friction_stiffness, cl_float cohesion_stiffness) {
    char filename[500];
    sprintf(filename, "%llu_%s%s_setup.txt", NUMPART, dir, prefix);
    FILE *fd = fopen(filename, "w");
    fprintf(fd, "Number of particles: %llu\n", NUMPART);
    fprintf(fd, "Timestep: %f\n", timestep);
    fprintf(fd, "Simulation length: %f\n", sim_length);
    fprintf(fd, "Domain length: %f\n", domain_length);
    fprintf(fd, "Stiffness: %f\n", stiffness);
    fprintf(fd, "Restitution coefficient: %f\n", restitution_coefficient);
    fprintf(fd, "Friction coefficient: %f\n", friction_coefficient);
    fprintf(fd, "Friction stiffness: %f\n", friction_stiffness);
    fprintf(fd, "Cohesion stiffness: %f\n", cohesion_stiffness);
    fclose(fd);
}

void writeTime(char prefix[], char dir[], cl_ulong NUMPART, char label[]) {
    char filename[500];
    sprintf(filename, "%s%llu_%s_setup.txt", dir, NUMPART, prefix);

    FILE *fd = fopen(filename, "a");

    time_t now;
    now = time(NULL);
    fprintf(fd, "%s: %s", label, ctime(&now));
    fclose(fd);
}

boolean checkDirExists(char dir[]) {
    const char *folderr;
    struct stat sb;

    if (stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return TRUE;
    } else {
        return FALSE;
    }
}