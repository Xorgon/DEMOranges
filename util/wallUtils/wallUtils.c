//
// Created by Elijah on 14/02/2018.
//

#include "wallUtils.h"


void generate_closed_box(aa_wall **walls, float length, cl_float3 center) {
    *walls = malloc(6 * sizeof(aa_wall));

    cl_float cx = center.x;
    cl_float cy = center.y;
    cl_float cz = center.z;

    (*walls)[0].min = (cl_float3) {cx - length / 2, cy - length / 2, cz - length / 2};
    (*walls)[0].max = (cl_float3) {cx - length / 2, cy + length / 2, cz + length / 2};
    (*walls)[0].normal = get_aa_wall_normal((*walls)[0]);

    (*walls)[1].min = (cl_float3) {cx - length / 2, cy - length / 2, cz - length / 2};
    (*walls)[1].max = (cl_float3) {cx + length / 2, cy - length / 2, cz + length / 2};
    (*walls)[1].normal = get_aa_wall_normal((*walls)[1]);

    (*walls)[2].min = (cl_float3) {cx - length / 2, cy - length / 2, cz - length / 2};
    (*walls)[2].max = (cl_float3) {cx + length / 2, cy + length / 2, cz - length / 2};
    (*walls)[2].normal = get_aa_wall_normal((*walls)[2]);

    (*walls)[3].min = (cl_float3) {cx + length / 2, cy - length / 2, cz - length / 2};
    (*walls)[3].max = (cl_float3) {cx + length / 2, cy + length / 2, cz + length / 2};
    (*walls)[3].normal = get_aa_wall_normal((*walls)[3]);

    (*walls)[4].min = (cl_float3) {cx - length / 2, cy + length / 2, cz - length / 2};
    (*walls)[4].max = (cl_float3) {cx + length / 2, cy + length / 2, cz + length / 2};
    (*walls)[4].normal = get_aa_wall_normal((*walls)[4]);

    (*walls)[5].min = (cl_float3) {cx - length / 2, cy - length / 2, cz + length / 2};
    (*walls)[5].max = (cl_float3) {cx + length / 2, cy + length / 2, cz + length / 2};
    (*walls)[5].normal = get_aa_wall_normal((*walls)[5]);
}

cl_float3 get_aa_wall_normal(aa_wall wall) {
    cl_float3 dif = subtract(wall.max, wall.min);
    if (dif.x == 0) {
        return (cl_float3) {1, 0, 0};
    } else if (dif.y == 0) {
        return (cl_float3) {0, 1, 0};
    } else if (dif.z == 0) {
        return (cl_float3) {0, 0, 1};
    } else {
        fprintf(stderr, "Error: AA Wall is not axis-aligned.");
        return (cl_float3) {1, 1, 1};
    }
}