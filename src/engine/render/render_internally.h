#ifndef RENDER_INTERNALLY_H
#define RENDER_INTERNALLY_H

#include <SDL3/SDL.h>

#include "engine/utils/types.h"


SDL_Window *render_init_window(u32 width, u32 height, string title);
void render_init_quad(u32 *vao, u32 *vbo, u32 *ebo);
void render_init_color_texture(u32 *texture);
void render_init_shaders(u32* default_shader, u32* shader_batch, float render_width, float render_height);
void render_init_line(u32* vao, u32* vbo);
void render_init_batch_quads(u32* vao, u32* vbo, u32* ebo);
u32 render_shader_create(const char *path_vert, const char *path_frag);

#endif /* RENDER_INTERNALLY_H */
