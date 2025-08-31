#include "sprite.h"
#include "engine/render/render.h"

#define LAST_ROW 29

float get_flip_sprite_rows(float row){
    return LAST_ROW - row;
}

Sprite sprite_init(Sprite_Sheet* sprite_sheet, float start_row, float start_col) {
    Sprite sprite = {
        .sprite_sheet = sprite_sheet,
        .start_row = get_flip_sprite_rows(start_row),
        .start_col = start_col,
        .current_row = get_flip_sprite_rows(start_row),
        .current_col = start_col,
    };

    return sprite;
}

void set_sprite_draw_col(Sprite* sprite, u8 current_frame) {
    sprite->current_col = sprite->start_col + current_frame;
}

float get_sprite_draw_col(Sprite* sprite, u8 current_frame) {
    set_sprite_draw_col(sprite, current_frame);
    return sprite->current_col;
}
