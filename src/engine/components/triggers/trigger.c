#include "trigger.h"


Trigger trigger_init(Trigger_Type type, bool is_triggerable_once){
    Trigger tigger = {
        .type = type,
        .is_triggered = false,
        .is_triggerable_once = is_triggerable_once
    };
    
    return tigger;
}


bool can_event(Trigger* trigger) {
    if(trigger->is_triggerable_once && trigger->is_triggered) {
        return false;
    }
    return true;
}
