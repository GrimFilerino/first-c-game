#ifndef SPRITE_H
#define SPRITE_H

#include "engine/render/render.h"

typedef struct sprite {
    Sprite_Sheet* sprite_sheet;
    float start_row;
    float start_col;
    float current_row;
    float current_col;
} Sprite;

Sprite sprite_init(Sprite_Sheet* sprite_sheet, float start_row, float start_col);
float get_sprite_draw_col(Sprite* sprite, u8 current_frame);
float get_flip_sprite_rows(float row);
void set_sprite_draw_col(Sprite* sprite, u8 current_frame);

#endif
