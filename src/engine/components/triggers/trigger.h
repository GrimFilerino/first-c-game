#ifndef TIGGER_H
#define TIGGER_H

#include "engine/components/sprite/sprite.h"
#include <stdbool.h>

typedef enum {
    TRIGGER_DAMAGE,
    TRIGGER_SPRITE_CHANGE
} Trigger_Type;


typedef struct trigger {
    Trigger_Type type; 
    bool is_triggered;
    bool is_triggerable_once;
} Trigger;


Trigger trigger_init(Trigger_Type type, bool is_triggerable_once);
bool can_event(Trigger* trigger);

#endif
