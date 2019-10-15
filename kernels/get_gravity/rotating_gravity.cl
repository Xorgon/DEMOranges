#define DEFAULT_PARTICLE_GRAVITY 0

float3 get_gravity(particle p, float t) {
    float t_fact = 0.05 * t * t;
    if (t > sqrt(16 * 2 * 3.14159265358979 / 0.05)) {
        t_fact = 0;
    }
    return (float3) {9.81 * sin(t_fact), -9.81 * cos(t_fact), 0};
}
