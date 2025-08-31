#include "engine/engine.h"
#include "engine/entities/entity_manager.h"
#include <glad/glad.h>
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include "../utils/util.h"
#include "render.h"
#include "engine/utils/lists/array_list.h"
#include "engine/utils/types.h"
#include "linmath.h"
#include "render_internally.h"

static float window_width= 960;
static float window_height = 540;
static float render_width = 320;
static float render_height = 180;
static float scale = 1;

static u32 vao_quad;
static u32 vbo_quad;
static u32 ebo_quad;
static u32 vao_line;
static u32 vbo_line;
static u32 shader_default;
static u32 texture_color;
static mat4x4 projection;

static u32 vao_batch;
static u32 vbo_batch;
static u32 ebo_batch;
static u32 shader_batch;
static Array_List* list_batch;

SDL_Window* render_init(){
    SDL_Window* window = render_init_window(window_width, window_height, "Biscut game");
    
    render_init_quad(&vao_quad, &vbo_quad, &ebo_quad);
    render_init_batch_quads(&vao_batch, &vbo_batch, &ebo_batch);
    render_init_line(&vao_line, &vbo_line);
    render_init_shaders(&shader_default,&shader_batch, render_width, render_height);
    render_init_color_texture(&texture_color);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    list_batch = array_list_create(sizeof(Batch_Vertex), 8);

    stbi_set_flip_vertically_on_load(1);

    return window;
}

void render_begin(){
    glClearColor(0.278f, 0.176f, 0.235f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    list_batch->length = 0;
}

static void render_batch(Batch_Vertex* vertices, size_t count, u32 texture_slots[8]) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_batch);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Batch_Vertex), vertices);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_color);
    
    for(u32 i = 1; i < 8; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture_slots[i]);
    }

    glUseProgram(shader_batch);
    glBindVertexArray(vao_batch);

    glDrawElements(GL_TRIANGLES, (count >> 2) * 6, GL_UNSIGNED_INT, NULL);
}

static void append_quad(float *pos, float *size, float *texture_cords, float *color, u32 texture_id) {
    vec4 uvs = {0,0,1,1};
    
    if(texture_cords != NULL) {
        memcpy(uvs, texture_cords, sizeof(vec4));
    }

    array_list_append(list_batch, &(Batch_Vertex){
        .position = { pos[0], pos[1] },
        .color = { color[0], color[1], color[2], color[3] },
        .uvs = { uvs[0], uvs[1] },
        .texture_id = texture_id,
    });

    array_list_append(list_batch, &(Batch_Vertex){
        .position = { pos[0] + size[0], pos[1] },
        .color = { color[0], color[1], color[2], color[3] },
        .uvs = { uvs[2], uvs[1] },
        .texture_id = texture_id,
    });

    array_list_append(list_batch, &(Batch_Vertex){
        .position = { pos[0] + size[0], pos[1] + size[1] },
        .color = { color[0], color[1], color[2], color[3] },
        .uvs = { uvs[2], uvs[3] },
        .texture_id = texture_id,
    });

    array_list_append(list_batch, &(Batch_Vertex){
        .position = { pos[0], pos[1] + size[1] },
        .color = { color[0], color[1], color[2], color[3] },
        .uvs = { uvs[0], uvs[3] },
        .texture_id = texture_id,
    });
}

void render_end(SDL_Window* window, u32 texture_slots[8]){
    render_batch(list_batch->items, list_batch->length, texture_slots);
    SDL_GL_SwapWindow(window);
}

void render_quad(vec2 pos, vec2 size, vec4 color){
    glUseProgram(shader_default);

    mat4x4 model;

    mat4x4_translate(model, pos[0], pos[1], 0);
    mat4x4_scale_aniso(model, model, size[0], size[1], 1);

    glUniformMatrix4fv(glGetUniformLocation(shader_default, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform4fv(glad_glGetUniformLocation(shader_default, "color"), 1, color);

    glBindVertexArray(vao_quad);
    glBindTexture(GL_TEXTURE_2D, texture_color);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
}

void render_quad_line(vec2 pos, vec2 size, vec4 color) {
	vec2 points[4] = {
		{pos[0] - size[0] * 0.5, pos[1] - size[1] * 0.5},
		{pos[0] + size[0] * 0.5, pos[1] - size[1] * 0.5},
		{pos[0] + size[0] * 0.5, pos[1] + size[1] * 0.5},
		{pos[0] - size[0] * 0.5, pos[1] + size[1] * 0.5},
	};

	render_line_segment(points[0], points[1], color);
	render_line_segment(points[1], points[2], color);
	render_line_segment(points[2], points[3], color);
	render_line_segment(points[3], points[0], color);
}

void render_line_segment(vec2 start, vec2 end, vec4 color){
    glUseProgram(shader_default);
    glLineWidth(3);

    float x = end[0] - start[0];
    float y = end[1] - start[1];
    float line[6] = {0,0,0,x,y,0};

    mat4x4 model;

    mat4x4_translate(model, start[0], start[1], 0);

    glUniformMatrix4fv(glGetUniformLocation(shader_default, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform4fv(glad_glGetUniformLocation(shader_default, "color"), 1, color);
    
    glBindTexture(GL_TEXTURE_2D, texture_color);
    glBindVertexArray(vao_quad);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_line);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line), line);
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
}

void render_aabb(float *aabb, vec4 color){
    vec2 size;
    vec2_scale(size, &aabb[2], 2);
    render_quad_line(&aabb[0], size, color);
}

float get_scale() {
    return scale;
}

void render_sprite_sheet_init(Sprite_Sheet* sprite_sheet, const char* path, float cell_width, float cell_height) {
    glGenTextures(1, &sprite_sheet->texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite_sheet->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, channel_count;
    u8* image_data = stbi_load(path, &width, &height, &channel_count, 0);
    
    if(!image_data) {
        ERROR_EXIT("Could not load image at path : %s\n", path);
    }
    
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA8,width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    
    stbi_image_free(image_data);

    sprite_sheet->width = (float)width;
    sprite_sheet->height = (float)height;
    sprite_sheet->cell_width = cell_width;
    sprite_sheet->cell_height = cell_height;
}

i32 find_texture_slot(u32 texture_slots[8], u32 texture_id) {
    for(i32 i = 1; i < 8; ++i) {
        if(texture_slots[i] == texture_id) {
            return i;
        }
    }

    return -1;
}

i32 try_insert_texture(u32 texture_slots[8], u32 texture_id) {
    i32 index = find_texture_slot(texture_slots, texture_id);
    if(index > 0) {
        return index;
    }

    for(i32 i = 1; i < 8; ++i) {
        if(texture_slots[i] == 0) {
            texture_slots[i] = texture_id;
            return i;
        }
    }

    return -1;
}

static void calculate_sprite_texture_coordinates(vec4 result, float row, float col, float tex_width, float tex_height, float cell_width, float cell_height){
    float w = 1.0 / (tex_width / cell_width);
    float h = 1.0 / (tex_height / cell_height);
    float x = col * w;
    float y = row * h;
    result[0] = x;
    result[1] = y;
    result[2] = x + w;
    result[3] = y + h;
}

void render_sprite_sheet_frame(Sprite_Sheet* sprite_sheet, float row, float col, vec2 pos, i16 dir, u32 texture_slots[8]){
    vec4 uvs;
    calculate_sprite_texture_coordinates(uvs, row, col, sprite_sheet->width, sprite_sheet->height, sprite_sheet->cell_width, sprite_sheet->cell_height);
  
    vec2 render_pos;
    vec2_sub(render_pos, pos, engine.camera.position);

    if (dir == LEFT || dir == DOWN) {
        float temp = uvs[0]; 
        uvs[0] = uvs[2]; 
        uvs[2] = temp;
    }


    vec2 size = {sprite_sheet->cell_width, sprite_sheet->cell_height};
    vec2 bottom_left = {render_pos[0] - size[0] * 0.5, render_pos[1] - size[1] * 0.5};

    i32 texture_slot = try_insert_texture(texture_slots, sprite_sheet->texture_id);
    if(texture_slot == -1) {
        printf("Failed texture stuff");
    }

    append_quad(bottom_left, size, uvs, WHITE, (float)texture_slot);
}
