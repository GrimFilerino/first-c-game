#include "camera.h"
#include "engine/entities/entity_manager.h"

Camera camera_init(u32 player_id) {
    return (Camera) {
        .position = {0, 0},
        .player_id = player_id,
    };
}


void update_camera(Camera* camera, Entity* player){
    camera->position[0] = player->aabb->position[0] - 160;
    camera->position[1] = player->aabb->position[1] - 75;
}
