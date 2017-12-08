/* Kernel to make collision list for particle-wall collisions. */

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
    float3 normal;
    float3 max;
    float3 min;
} aa_wall;

__kernel void make_pw_collisions(
) {}