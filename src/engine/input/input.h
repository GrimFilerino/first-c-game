#ifndef INPUT_H
#define INPUT_H

#include "engine/entities/entity_manager.h"
typedef enum input_type {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ESCAPE,
    SNEAK,
    ATTACK,
} Input_Type;

typedef enum key_state {
    KEY_RELEASED,
    KEY_PRESSED,
    KEY_HELD
} Key_State;

typedef struct input_state {
    Key_State move_up;
    Key_State move_down;
    Key_State move_left;
    Key_State move_right;
    Key_State escape;
    Key_State sneak;
    Key_State attack;
} Input_State;

void input_update();
void input_handle(Entity* player_entity);

#endif
