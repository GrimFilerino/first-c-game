#include "physics.h"
#include "engine/components/aabb/aabb.h"
#include "engine/engine.h"


Physics_Body physics_body_create() {
    Physics_Body physics_body = {
        .velocity = { 0, 0 },
        .is_moving_left = 0
    };

    return physics_body;
}

void physics_body_update(Physics_Body* physics_body, AABB* aabb) {
    aabb->position[0] += physics_body->velocity[0] * engine.time.delta;
    aabb->position[1] += physics_body->velocity[1] * engine.time.delta;

    physics_body->velocity[0] = 0;
    physics_body->velocity[1] = 0;
}

