//
// Created by Elijah on 04/12/2017.
//

#ifndef DEMORANGES_COLLISION_H
#define DEMORANGES_COLLISION_H

#include <CL/cl_platform.h>

typedef struct pp_collision {
    cl_ulong p1_id;
    cl_ulong p2_id;

    cl_float stiffness;
    cl_float damping_coefficient;
    cl_float friction_coefficient;
    cl_float friction_stiffness;
//    cl_char padding[96];
// Structure memory alignment for Visual Studio and GCC compilers.
#if defined(_MSC_VER)
} __declspec(align(32)) pp_collision;
#elif defined(__GNUC) || defined(__GNUG__ ) || defined(__MINGW_GCC_VERSION)
} __attribute__((aligned (32))) pp_collision;
#endif


#endif //DEMORANGES_COLLISION_H
