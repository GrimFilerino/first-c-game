#include "input.h"
#include "../engine.h"
#include "../utils/types.h"
#include "SDL3/SDL_keyboard.h"
#include "engine/utils/util.h"
#include "linmath.h"
#include <math.h>

static void update_key_state(u8 current_state, Key_State *input_state) {
    if(current_state) {
        if(*input_state > 0) {
            *input_state = KEY_HELD;
        } else {
            *input_state = KEY_PRESSED;
        }
    } else {
        *input_state = KEY_RELEASED;
    }
}

void input_update() {
    const bool* keyboard_state = SDL_GetKeyboardState(NULL);
    update_key_state(keyboard_state[engine.config.key_binds[MOVE_UP]], &engine.input.move_up);
    update_key_state(keyboard_state[engine.config.key_binds[MOVE_DOWN]], &engine.input.move_down);
    update_key_state(keyboard_state[engine.config.key_binds[MOVE_LEFT]], &engine.input.move_left);
    update_key_state(keyboard_state[engine.config.key_binds[MOVE_RIGHT]], &engine.input.move_right);
    update_key_state(keyboard_state[engine.config.key_binds[ESCAPE]], &engine.input.escape);
    update_key_state(keyboard_state[engine.config.key_binds[SNEAK]], &engine.input.sneak);
    update_key_state(keyboard_state[engine.config.key_binds[ATTACK]], &engine.input.attack);
}


void input_handle(Entity* player_entity){
    float velx = 0;
    float vely = 0;
    float base_speed = 60.0;
    float sneak_speed = 30.0;
    float movement_speed_multiplier = 1.0;
        
    if(engine.input.escape) engine.is_running = false;

    if(engine.input.sneak) base_speed = sneak_speed;

    if(engine.input.move_left > 0) velx -= 1;
    if(engine.input.move_right > 0) velx += 1;
    if(engine.input.move_up > 0) vely += 1;
    if(engine.input.move_down > 0) vely -= 1;

    vec2 dir = { velx, vely };
    float len = sqrtf(dir[0]*dir[0] + dir[1]*dir[1]);
    if (len > 0) {
        dir[0] /= len;
        dir[1] /= len;
    }    

    if(dir[0] != 0 || dir[1] != 0) {
        player_entity->physics_body->velocity[0] = dir[0] * (base_speed * movement_speed_multiplier);
        player_entity->physics_body->velocity[1] = dir[1] * (base_speed * movement_speed_multiplier);
    }
}
