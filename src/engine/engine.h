#ifndef ENGINE_H
#define ENGINE_H

#include "engine/camera/camera.h"
#include "engine/entities/entity_manager.h"
#include "engine/input/input.h"
#include "engine/utils/types.h"
#include "utils/time/time.h"
#include "config/config.h"

typedef struct engine {
    Config_State config;
    Input_State input;
    Time_State time;
    Entity_Manager_State entity_manager;
    Camera camera;
    bool is_running;
    u32 texture_slots[8];
    string current_scene;
} Engine;

extern Engine engine;

void engine_init(void);

#endif /* ENGINE_H */

