#include "config.h"
#include "../utils/io/io.h"
#include "../utils/util.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_scancode.h"
#include "engine/engine.h"
#include "engine/input/input.h"
#include <string.h>

static const char* CONFIG_DEFAULT =
    "[controls]\n"
    "up = W\n"
    "down = S\n"
    "left = a\n"
    "right = d\n"
    "escape = Escape\n"
    "sneak = Left SHIFT\n"
    "attack = F\n"
    "\n";

static char tmp_buffer[200] = {0};

static char *config_get_value(const char *config_buffer, const char* value) {
    char *line = strstr(config_buffer, value);

    if(!line) {
        ERROR_EXIT("Could not find config value %s. Try deleting config.ini and rerun the game!", value);
    }

    size_t len = strlen(line);
    char *end = line + len;
    
    char *current = line;
    char *tmp_ptr = &tmp_buffer[0];

    while(*current != '=' && current != end) {
        ++current;
    }

    ++current;

    while(*current == ' '){
        ++current;
    }

    while(*current != '\n' && *current != 0 && current != end) {
        *tmp_ptr++ = *current++;
    }

    *tmp_ptr = 0;

    return tmp_buffer;
}

static void load_controls(const char* config_buffer) {
    config_bind_key(MOVE_UP, config_get_value(config_buffer, "up"));
    config_bind_key(MOVE_DOWN, config_get_value(config_buffer, "down"));
    config_bind_key(MOVE_LEFT, config_get_value(config_buffer, "left"));
    config_bind_key(MOVE_RIGHT, config_get_value(config_buffer, "right"));
    config_bind_key(ESCAPE, config_get_value(config_buffer, "escape"));
    config_bind_key(SNEAK, config_get_value(config_buffer, "sneak"));
    config_bind_key(ATTACK, config_get_value(config_buffer, "attack"));
}

static int config_load() {
    File file_config = io_file_read("./config.ini");
    if(!file_config.is_valid) {
        return 1;
    }

    load_controls(file_config.data);

    free(file_config.data);

    return 0;
}

void config_init() {
    if(config_load() == 0) {
        return;
    }

    io_file_write((void*)CONFIG_DEFAULT, strlen(CONFIG_DEFAULT), "./config.ini");

    if(config_load() != 0) {
        ERROR_EXIT("Could not create or load config file.\n");
    }
}


void config_bind_key(Input_Type key, const char *key_name) {
    SDL_Scancode scan_code = SDL_GetScancodeFromName(key_name);

    if(scan_code == SDL_SCANCODE_UNKNOWN) {
        ERROR_RETURN(, "Invalid scan code when binding key : %s\n", key_name);
    }

    engine.config.key_binds[key] = scan_code;
}
