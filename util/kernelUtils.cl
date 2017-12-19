typedef struct {
    float3 pos;
    float3 vel;
    float3 forces;
    ulong particle_id;
    float particle_diameter;
    float density;
    float fluid_viscosity;
} __attribute__ ((aligned (128))) particle;

typedef struct {
    particle p1;
    particle p2;

    float stiffness;
    float damping_coefficient;
    float friction_coefficient;
    float friction_stiffness;
} particle_collision;

typedef struct {
    float3 normal;
    float3 max;
    float3 min;
} aa_wall;

void atomicAdd_g_f(volatile __global float *addr, float val) {
   union {
       unsigned int u32;
       float f32;
   } next, expected, current;
   current.f32 = *addr;
   do {
       expected.f32 = current.f32;
       next.f32 = expected.f32 + val;
       current.u32 = atomic_cmpxchg((volatile __global unsigned int *)addr, expected.u32, next.u32);
   } while(current.u32 != expected.u32);
}