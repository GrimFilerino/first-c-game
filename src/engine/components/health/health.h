#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
    float max_health;
    float current_health;
} Health;

Health health_init(float health);
void heal(Health* h, float amount);

#endif
