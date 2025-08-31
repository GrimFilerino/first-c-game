#ifndef CAMERA_H
#define CAMERA_H

#include "engine/entities/entity_manager.h"
#include "engine/utils/types.h"
#include "linmath.h"

typedef struct camera {
    vec2 position;
    u32 player_id;
} Camera;

Camera camera_init(u32 player_id);
void update_camera(Camera* camera, Entity* player);

#endif
