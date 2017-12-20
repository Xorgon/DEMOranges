//
// Created by Elijah on 19/12/2017.
//

#ifndef DEMORANGES_TEST_ALIGNMENT_H
#define DEMORANGES_TEST_ALIGNMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <conio.h>
#include "../../util/clUtils.h"
#include "../../structures/particle.h"
#include "../../structures/collision.h"

boolean test_particle_struct_alignment(boolean verbose);
boolean test_pp_collision_struct_alignment(boolean verbose);
boolean test_wall_struct_alignment(boolean verbose);

#endif //DEMORANGES_TEST_ALIGNMENT_H
