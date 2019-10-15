#define DEFAULT_PARTICLE_FLUID_VEL 0

#define MAX_FLOW_SPEED 0.5
#define VORTEX_FREQ 3

float3 get_vel_fluid(particle p, float t) {
    float3 vel_fluid = (float3) {0, 0, 0};
    float3 offset_pos = p.pos + (M_PI_F / (VORTEX_FREQ * 2)) * (float3) {1, 1, 1};
    vel_fluid.x = MAX_FLOW_SPEED * cos(VORTEX_FREQ * offset_pos.x) * sin(VORTEX_FREQ * offset_pos.y) * sin(VORTEX_FREQ * offset_pos.z);
    vel_fluid.y = MAX_FLOW_SPEED * sin(VORTEX_FREQ * offset_pos.x) * cos(VORTEX_FREQ * offset_pos.y) * sin(VORTEX_FREQ * offset_pos.z);
    vel_fluid.z = - 2 * MAX_FLOW_SPEED * cos(VORTEX_FREQ * offset_pos.x) * sin(VORTEX_FREQ * offset_pos.y) * cos(VORTEX_FREQ * offset_pos.z);
    return vel_fluid;
}
