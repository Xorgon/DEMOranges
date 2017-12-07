/* Kernel to iterate particles. */

typedef struct {
    ulong particle_id;
    float particle_diameter;
    float density;
    float *fluid_viscosity;

    float3 pos;
    float3 vel;
    float3 forces;
} particle;

__kernel void iterate_particle(
) {}