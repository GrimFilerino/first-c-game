#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#include "render_internally.h"
#include "linmath.h"
#include "render.h"

#include "../utils/types.h"
#include "../utils/util.h"
#include "../utils/io/io.h"

SDL_Window *render_init_window(u32 width, u32 height, string title) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not init SDL :  %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("Test", width, height, SDL_WINDOW_OPENGL);

    if(!window) {
        printf("Could not create window : %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_CreateContext(window);

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Could not load GL : %s\n", SDL_GetError());
        exit(1);
    }
    
    return window;
}

void render_init_quad(u32 *vao, u32 *vbo, u32 *ebo) {
    //  x     y,   z, u, v
    float vertices[] = {
        0.5,  0.5, 0, 0, 0,
        0.5, -0.5, 0, 0, 1,
       -0.5, -0.5, 0, 1, 1,
       -0.5,  0.5, 0, 1, 0,
    };

    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}


void render_init_shaders(u32* shader_default, u32* shader_batch, float render_width, float render_height) {
    mat4x4 projection;
    *shader_default = render_shader_create("./assets/shaders/default.vert", "./assets/shaders/default.frag");
    *shader_batch = render_shader_create("./assets/shaders/batch_quad.vert", "./assets/shaders/batch_quad.frag");

    mat4x4_ortho(projection, 0, render_width, 0, render_height, -2, 2);

    glUseProgram(*shader_default);
    glUniformMatrix4fv(
        glGetUniformLocation(*shader_default, "projection"),
        1,
        GL_FALSE,
        &projection[0][0]
    );

    glUseProgram(*shader_batch);
    glUniformMatrix4fv(
        glGetUniformLocation(*shader_batch, "projection"),
        1,
        GL_FALSE,
        &projection[0][0]
    );

    for(u32 i = 0; i < 8; ++i) {
        char name[] = "texture_slot_N";
        sprintf(name,"texture_slot_%u", i);
        glUniform1i(glGetUniformLocation(*shader_batch, name), i);
    }
}

void render_init_color_texture(u32 *texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    u8 solid_white[4] = {255,255,255,255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, solid_white);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void render_init_line(u32* vao, u32* vbo){
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void render_init_batch_quads(u32 *vao, u32 *vbo, u32 *ebo){
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    u32 indices[MAX_BATCH_ELEMENTS];
    for (u32 i = 0, offset = 0; i < MAX_BATCH_ELEMENTS; i+=6, offset += 4) {
        indices[i + 0] = offset + 0; 
        indices[i + 1] = offset + 1; 
        indices[i + 2] = offset + 2; 
        indices[i + 3] = offset + 2; 
        indices[i + 4] = offset + 3; 
        indices[i + 5] = offset + 0; 
    }

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_VERTICES * sizeof(Batch_Vertex), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Batch_Vertex), (void*) offsetof(Batch_Vertex, position));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Batch_Vertex), (void*) offsetof(Batch_Vertex, uvs));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Batch_Vertex), (void*) offsetof(Batch_Vertex, color));
   
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Batch_Vertex), (void*) offsetof(Batch_Vertex, texture_id));
    
    glGenBuffers(1, ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_BATCH_ELEMENTS * sizeof(u32), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

u32 render_shader_create(const char *path_vert, const char *path_frag) {
    int success;
    char log[512];

    File file_vertex = io_file_read(path_vert);
    if(!file_vertex.is_valid) {
        ERROR_EXIT("Error reading shader: %s\n", path_vert);
    }

    u32 shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_vertex, 1, (const char *const *)&file_vertex.data, NULL);
    glCompileShader(shader_vertex);
    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(shader_vertex, 512, NULL, log);
        ERROR_EXIT("Error compiling vertex shader: %s\n", log);
    }

    File file_fragment = io_file_read(path_frag);
    if(!file_fragment.is_valid) {
        ERROR_EXIT("Error reading shader: %s\n", path_frag);
    }

    u32 shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_fragment, 1, (const char *const *)&file_fragment.data, NULL);
    glCompileShader(shader_fragment);
    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(shader_fragment, 512, NULL, log);
        ERROR_EXIT("Error compiling fragment shader: %s\n", log);
    }

    u32 shader = glCreateProgram();
    glAttachShader(shader, shader_vertex);
    glAttachShader(shader, shader_fragment);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    
    if(!success) {
        glGetProgramInfoLog(shader, 512, NULL, log);
        ERROR_EXIT("Error linking shader: %s\n", log);
    }

    free(file_vertex.data);
    free(file_fragment.data);

    return shader;
}
