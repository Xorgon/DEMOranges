/* Kernel to calculate particle-particle collisions. */

typedef struct {
    ulong particle_id;
    float particle_diameter;
    float density;
    float fluid_viscosity;

    float3 pos;
    float3 vel;
    float3 forces;
} particle;

typedef struct {
    particle p1;
    particle p2;

    float stiffness;
    float damping_coefficient;
    float friction_coefficient;
    float friction_stiffness;
} particle_collision;

__kernel void pp_collision(
) {}