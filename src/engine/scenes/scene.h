#ifndef SCENE_H
#define SCENE_H

#include "engine/components/aabb/aabb.h"
#include "engine/render/render.h"
#include "engine/utils/lists/array_list.h"
#include "engine/utils/types.h"
#include "linmath.h"
#include <stdbool.h>

#define MAX_TILES 20000;


typedef struct scene {
    string unique_name;
    string config;
    bool is_running;
} Scene;

typedef struct {
    Scene base_scene;
    Array_List* buttons;
} Scene_Menu;

typedef struct scene_play {
    Scene base_scene;
    u32 room_index;
    u32 spawn_enemy_timer;
    u32 current_score;
} Scene_Play;


Scene create_scene();

void scene_init(string current_scene);
void scene_update(string current_scene);
void scene_render(string current_scene);
void scene_end(string current_scene, string next_scene);


void scene_play_init();
void scene_play_update();
void scene_play_render();
void scene_play_end(string next_scene);

#endif
