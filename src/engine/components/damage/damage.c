#include "damage.h"
#include "engine/components/health/health.h"
#include "engine/engine.h"



Damage damage_init(float damage_value, float aoe, bool is_over_time, float damage_time, float damage_cooldown) {
    Damage d = {
        .damage_value = damage_value,
        .area_of_effect = aoe,
        .is_over_time = is_over_time,
        .damage_time = damage_time,
        .damage_cooldown = damage_cooldown,
        .last_damage_time = 0
    };

    return d;
}

void do_damage(Health* entity_health, Damage* d) {
    if(entity_health->current_health <= 0) {
        entity_health->current_health = 0;
        return;
    }


    if (engine.time.now - d->last_damage_time < d->damage_cooldown) {
        return;
    }
    
    entity_health->current_health -= d->damage_value;

    if (entity_health->current_health < 0) {
        entity_health->current_health = 0;
    }

    d->last_damage_time = engine.time.now;
}
