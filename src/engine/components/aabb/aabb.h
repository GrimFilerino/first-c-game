#ifndef AABB_H
#define AABB_H

#include "linmath.h"
#include <stdbool.h>

typedef struct {
    vec2 pos;
    vec2 size;
    bool is_kinematic;
} AABBData;

typedef struct aabb {
    vec2 position;
    vec2 half_size;
} AABB;

typedef struct hit {
    bool is_hit;
    float time;
    vec2 position;
    vec2 normal;
} Hit;

AABB aabb_create(vec2 pos, vec2 size);
bool is_boxes_colliding(AABB* a, AABB* b);

#endif
