//
// Created by Elijah on 14/02/2018.
//

#include "wallUtils.h"
#include "../../structures/wall.h"


void generate_closed_box(aa_wall **walls, float length, cl_float3 center) {
    *walls = malloc(6 * sizeof(aa_wall));

    generate_box_geometry(*walls, length, center);
}

int generate_hourglass(aa_wall **walls, float height, float gap_width) {
    *walls = malloc(12 * sizeof(aa_wall));

    float aa_gap_height = gap_width / sqrtf(2);
    float box_length = height / (2 * sqrtf(2)) + aa_gap_height / 2;

    printf("Box length = %f, offset = %f, gap offset = %f\n", box_length, 0.5 * (box_length - aa_gap_height),
           aa_gap_height);

    cl_float3 top_box_center = (cl_float3) {0.5 * (box_length - aa_gap_height), 0.5 * (box_length - aa_gap_height), 0};
    generate_box_geometry(*walls, box_length, top_box_center);

    cl_float3 bot_box_center = (cl_float3) {-0.5 * (box_length - aa_gap_height), -0.5 * (box_length - aa_gap_height),
                                            0};
    generate_box_geometry(&((*walls)[6]), box_length, bot_box_center);

    // Create gap in top box.
    (*walls)[0].min.y += aa_gap_height;
    (*walls)[1].min.x += aa_gap_height;

    // Create gap in bottom box.
    (*walls)[9].max.y -= aa_gap_height;
    (*walls)[10].max.x -= aa_gap_height;

    // Move necessary wall to spare space.
    (*walls)[8] = (*walls)[10];

    // Make the top box sides cover the whole arrangement.
    (*walls)[2].min = (cl_float3) {-(box_length - 0.5 * aa_gap_height), -(box_length - 0.5 * aa_gap_height), -box_length / 2};
    (*walls)[2].max = (cl_float3) {(box_length - 0.5 * aa_gap_height), (box_length - 0.5 * aa_gap_height), -box_length / 2};

    (*walls)[5].min = (cl_float3) {-(box_length - 0.5 * aa_gap_height), -(box_length - 0.5 * aa_gap_height), box_length / 2};
    (*walls)[5].max = (cl_float3) {(box_length - 0.5 * aa_gap_height), (box_length - 0.5 * aa_gap_height), box_length / 2};
    return 10;
}

void generate_box_geometry(aa_wall *walls, float length, cl_float3 center) {
    cl_float cx = center.x;
    cl_float cy = center.y;
    cl_float cz = center.z;

    (walls)[0].min = (cl_float3) {cx - length / 2, cy - length / 2, cz - length / 2};
    (walls)[0].max = (cl_float3) {cx - length / 2, cy + length / 2, cz + length / 2};
    (walls)[0].normal = get_aa_wall_normal((walls)[0]);

    (walls)[1].min = (cl_float3) {cx - length / 2, cy - length / 2, cz - length / 2};
    (walls)[1].max = (cl_float3) {cx + length / 2, cy - length / 2, cz + length / 2};
    (walls)[1].normal = get_aa_wall_normal((walls)[1]);

    (walls)[2].min = (cl_float3) {cx - length / 2, cy - length / 2, cz - length / 2};
    (walls)[2].max = (cl_float3) {cx + length / 2, cy + length / 2, cz - length / 2};
    (walls)[2].normal = get_aa_wall_normal((walls)[2]);

    (walls)[3].min = (cl_float3) {cx + length / 2, cy - length / 2, cz - length / 2};
    (walls)[3].max = (cl_float3) {cx + length / 2, cy + length / 2, cz + length / 2};
    (walls)[3].normal = get_aa_wall_normal((walls)[3]);

    (walls)[4].min = (cl_float3) {cx - length / 2, cy + length / 2, cz - length / 2};
    (walls)[4].max = (cl_float3) {cx + length / 2, cy + length / 2, cz + length / 2};
    (walls)[4].normal = get_aa_wall_normal((walls)[4]);

    (walls)[5].min = (cl_float3) {cx - length / 2, cy - length / 2, cz + length / 2};
    (walls)[5].max = (cl_float3) {cx + length / 2, cy + length / 2, cz + length / 2};
    (walls)[5].normal = get_aa_wall_normal((walls)[5]);
}

void translate_walls(aa_wall *walls, int num_walls, cl_float3 trans_vect) {
    for (int i = 0; i < num_walls; i++) {
        walls[i].min.x += trans_vect.x;
        walls[i].min.y += trans_vect.y;
        walls[i].min.z += trans_vect.z;

        walls[i].max.x += trans_vect.x;
        walls[i].max.y += trans_vect.y;
        walls[i].max.z += trans_vect.z;
    }
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
        fprintf(stderr, "Error: AA Wall is not axis-aligned.\n");
        return (cl_float3) {1, 1, 1};
    }
}

void print_walls(aa_wall *walls, cl_ulong NUMWALLS) {
    for (int i = 0; i < NUMWALLS; i++) {
        printf("%i: min = (%f, %f, %f), max = (%f, %f, %f)\n", i, walls[i].min.x, walls[i].min.y, walls[i].min.z,
               walls[i].max.x, walls[i].max.y, walls[i].max.z);
    }
}