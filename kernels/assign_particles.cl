/* Kernel to assign particles to control volumes. */

typedef struct {
    ulong particle_id;
    float particle_diameter;
    float density;
    float fluid_viscosity;

    float3 pos;
    float3 vel;
    float3 forces;
} particle;

__kernel void assign_particles(
) {}