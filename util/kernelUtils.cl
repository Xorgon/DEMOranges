#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable // Enable 64 bit integer atomics.

typedef struct {
    float3 pos;
    float3 vel;
    float3 forces;
    ulong id;
    ulong cv_array_idx;
    float diameter;
    float effect_diameter;
    float density;
    float fluid_viscosity;
} __attribute__ ((aligned (128))) particle;

typedef struct {
    ulong p1_id;
    ulong p2_id;
    char cross_boundary_x;
    char cross_boundary_y;
    char cross_boundary_z;

//    float stiffness;
//    float damping_coefficient;
//    float friction_coefficient;
//    float friction_stiffness;
} __attribute__ ((aligned (16))) pp_collision;

typedef struct {
    ulong p_id;
    ulong w_id;

//    float stiffness;
//    float damping_coefficient;
//    float friction_coefficient;
//    float friction_stiffness;
} __attribute__ ((aligned (16))) pw_collision;

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

void atomicAdd_float3(volatile __global float3 *addr, float3 val) {
   union {
       volatile __global float *array[4];
       volatile __global float3 *vect;
   } vector;
   union {
       float array[4];
       float3 vect;
   } to_add;
   vector.vect = addr;
   to_add.vect = val;
   for (int i = 0; i < 3; i++) {
       atomicAdd_g_f(&(*vector.array)[i], to_add.array[i]);
   }
}

int3 pos_to_cv_coords(float3 pos, float domain_length, float cv_length){
    if (fabs(pos.x) > domain_length / 2 || fabs(pos.x) > domain_length / 2 || fabs(pos.x) > domain_length / 2) {
        printf("Failed to convert position to CV coordinates. Position outside of domain. [%f, %f, %f].\n",
                pos.x, pos.y, pos.z);
        return (int3) {-1, -1, -1};
    }
    int cv_x = floor((pos.x + domain_length / 2) / cv_length);
    int cv_y = floor((pos.y + domain_length / 2) / cv_length);
    int cv_z = floor((pos.z + domain_length / 2) / cv_length);
    return (int3) {cv_x, cv_y, cv_z};
}

ulong cv_coords_to_cv_array_idx(int3 cv_coords, int cvs_per_edge) {
    return cv_coords.x + cv_coords.y * cvs_per_edge + cv_coords.z * cvs_per_edge * cvs_per_edge;
}

int3 cv_array_idx_to_cv_coords(ulong cv_array_idx, int cvs_per_edge) {
    int x = cv_array_idx % cvs_per_edge;
    int y = ((cv_array_idx - x) / cvs_per_edge) % cvs_per_edge;
    int z = (cv_array_idx - x - y * cvs_per_edge) / cvs_per_edge / cvs_per_edge;
    int3 cv_coords = (int3) {x, y, z};
    return cv_coords;
}

float get_particle_mass(particle p) {
    if (p.density == -1) {
        return -1;
    } else {
        return (float) p.density * M_PI_F * pow(p.diameter, 3) / 6;
    }
}

float get_reduced_particle_mass(float m1, float m2) {
    if (m1 == -1 && m2 != -1) {
        return m2;
    } else if (m2 == -1 && m1 != -1) {
        return m1;
    } else if (m1 == -1 && m2 == -1) {
        printf("Warning: Colliding particles with infinite densities.");
        return 1e99;
    } else {
        return m1 * m2 / (m1 + m2);
    }
}

float get_damping_coefficient(float restitution_coeff, float stiffness, float reduced_particle_mass) {
    float ln_rest = log(restitution_coeff);
    return -2 * ln_rest * pow((float)(reduced_particle_mass * stiffness / (M_PI_F * M_PI_F + ln_rest * ln_rest)),
                                (float) 0.5);
}
