#include "engine/camera/camera.h"
#include "engine/engine.h"
#include "engine/entities/entity_manager.h"
#include "engine/render/render.h"
#include "scene.h"

Entity* player;
Scene_Play play;

static Sprite_Sheet sprite_sheet;
static Sprite_Sheet tile_sheet;
static Sprite_Sheet top_gui;

void scene_play_init() {
    
    /*play = (Scene_Play) {
        .base_scene 
    };*/

    render_sprite_sheet_init(&sprite_sheet, "./assets/sprites/sprite_sheet.png", 16, 16);
    render_sprite_sheet_init(&top_gui, "./assets/sprites/top_gui_biscuit.png", 320,64);

    render_sprite_sheet_init(&tile_sheet, "./assets/tile_maps/test.png", 320,192);

    player = entity_create_player(engine.entity_manager.next_id, (vec2){ 160, 90}, (vec2){ 16, 16}, &sprite_sheet, (vec2){0, 0} );
    Entity* enemy = entity_create_enemy(engine.entity_manager.next_id, ENTITY_CENTRY, (vec2){ 100, 100}, (vec2){ 16, 16}, &sprite_sheet, (vec2){0, 6} );

    engine.camera = camera_init(player->id);
}

void scene_play_update() {

    input_handle(player);

    update_camera(&engine.camera, player);

    //draw map
    render_sprite_sheet_frame(&tile_sheet, 0, 0, (vec2){160,90}, NO_DIR, engine.texture_slots);

    update_entities(engine.texture_slots);

    render_sprite_sheet_frame(&top_gui, 0, 0, (vec2){ engine.camera.position[0] + 160, engine.camera.position[1] + 150}, NO_DIR, engine.texture_slots);
    render_sprite_sheet_frame(&sprite_sheet, 3, 1,(vec2){ engine.camera.position[0] + 160, engine.camera.position[1] + 50}, NO_DIR, engine.texture_slots);

    destroy_entities();
}

void scene_play_render() {

}

void scene_play_end(string next_scene){

}

static void spawn_enemy() {
}

