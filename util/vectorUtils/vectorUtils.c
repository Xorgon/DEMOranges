//
// Created by Elijah on 14/02/2018.
//

#include "vectorUtils.h"

cl_float3 normalize(cl_float3 vect) {
    float mag = (float) sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
    return (cl_float3) {vect.x / mag, vect.y / mag, vect.z / mag};
}


cl_float3 subtract(cl_float3 v1, cl_float3 v2) {
    return (cl_float3) {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}