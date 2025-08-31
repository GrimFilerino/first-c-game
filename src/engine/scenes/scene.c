#include "scene.h"

void scene_init(string current_scene) {
    scene_play_init();
}

void scene_update(string current_scene) {
    scene_play_update();

    scene_render(current_scene);
}

void scene_render(string current_scene) {
    scene_play_render();
}

void scene_end(string current_scene, string next_scene) {
    
}
