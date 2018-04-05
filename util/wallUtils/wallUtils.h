//
// Created by Elijah on 14/02/2018.
//

#include "../../structures/wall.h"
#include <math.h>
#include <CL/cl.h>
#include "../vectorUtils/vectorUtils.h"
#include <stdio.h>

#ifndef DEMORANGES_WALLUTILS_H
#define DEMORANGES_WALLUTILS_H

void generate_closed_box(aa_wall **walls, float side_length, cl_float3 center);

void generate_box_geometry(aa_wall *walls, float length, cl_float3 center);

int generate_hourglass(aa_wall **walls, float height, float gap_width);

void translate_walls(aa_wall *walls, int num_walls, cl_float3 trans_vect);

cl_float3 get_aa_wall_normal(aa_wall wall);

void print_walls(aa_wall *walls, cl_ulong NUMWALLS);

#endif //DEMORANGES_WALLUTILS_H
