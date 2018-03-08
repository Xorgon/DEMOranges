#define DEFAULT_PARTICLE_FLUID_VEL 0

float3 get_vel_fluid(particle p, float delta_t) {
    return (float3) {1, 0, 0};
}
