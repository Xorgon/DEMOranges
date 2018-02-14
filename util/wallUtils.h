//
// Created by Elijah on 14/02/2018.
//

#include "../structures/wall.h"
#include <math.h>
#include <CL/cl.h>
#include "vectorUtils.h"
#include <stdio.h>

#ifndef DEMORANGES_WALLUTILS_H
#define DEMORANGES_WALLUTILS_H

void generate_closed_box(aa_wall **walls, float side_length);

cl_float3 get_aa_wall_normal(aa_wall wall);

#endif //DEMORANGES_WALLUTILS_H
