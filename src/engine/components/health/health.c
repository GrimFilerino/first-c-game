#include "health.h"


Health health_init(float health) {
    Health h = {
        .max_health = health,
        .current_health = health
    };

    return h;
}

void heal(Health* h, float amount) {

    if(h->current_health >= h->max_health) {
        h->current_health = h->max_health;
    } else {
        h->current_health += amount;
    }
}
