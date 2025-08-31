#ifndef RENDER_H
#define RENDER_H

#include "engine/utils/types.h"
#include <SDL3/SDL.h>
#include <linmath.h>

typedef struct batch_vertex {
    vec2 position;
    vec2 uvs;
    vec4 color;
    float texture_id;
} Batch_Vertex;

typedef struct sprite_sheet {
    float width;
    float height;
    float cell_width;
    float cell_height;
    u32 texture_id;
} Sprite_Sheet;

#define MAX_BATCH_QUADS 10000
#define MAX_BATCH_VERTICES 40000
#define MAX_BATCH_ELEMENTS 60000

SDL_Window* render_init();
void render_begin();
void render_end(SDL_Window* window, u32 texture_slots[8]);
void render_quad(vec2 pos, vec2 size, vec4 color);
void render_quad_line(vec2 pos, vec2 size, vec4 color);
void render_line_segment(vec2 start, vec2 end, vec4 color);
void render_aabb(float *aabb, vec4 color);
float get_scale();

void render_sprite_sheet_init(Sprite_Sheet* sprite_sheet, const char* path, float cell_width, float cell_height);
void render_sprite_sheet_frame(Sprite_Sheet* sprite_sheet, float row, float col, vec2 pos, i16 dir, u32 texture_slots[8]);

#endif /* RENDER_H */
