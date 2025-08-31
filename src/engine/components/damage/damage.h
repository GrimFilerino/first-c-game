#ifndef DAMAGE_H
#define DAMAGE_H

#include "engine/components/health/health.h"
#include <stdbool.h>

typedef struct damage {
    float damage_value;
    float area_of_effect;
    bool is_over_time;
    float damage_time;
    float damage_cooldown;
    float last_damage_time;
} Damage;

Damage damage_init(float damage_value, float aoe, bool is_over_time, float damage_time, float damage_cooldown);
void do_damage(Health* entity_health, Damage* d);

#endif
