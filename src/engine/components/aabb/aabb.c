#include "aabb.h"
#include "linmath.h"
#include <math.h>
#include <stdbool.h>

AABB aabb_create(vec2 pos, vec2 size) {
    AABB aabb = {
        .position = { pos[0], pos[1] },
        .half_size = { size[0] * 0.5, size[1] * 0.5 },
    };

    return aabb;
}

bool is_boxes_colliding(AABB* a, AABB* b) {
    float dx = fabsf(a->position[0] - b->position[0]);
    float dy = fabsf(a->position[1] - b->position[1]);

    float combined_width  = a->half_size[0] + b->half_size[0];
    float combined_height = a->half_size[1] + b->half_size[1];

    return (dx < combined_width) && (dy < combined_height);
}

