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

