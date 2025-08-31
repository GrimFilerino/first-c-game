#ifndef PHYSICS_H
#define PHYSICS_H

#include "engine/components/aabb/aabb.h"
#include "engine/utils/types.h"
#include "linmath.h"
#include <stdbool.h>

typedef struct physics_body {
    vec2 velocity;
    u8 is_moving_left;
} Physics_Body;

Physics_Body physics_body_create();
void physics_body_update(Physics_Body* physics_body, AABB* aabb);

#endif
