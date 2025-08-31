#include "entity.h"
#include "engine/components/aabb/aabb.h"
#include "engine/components/damage/damage.h"
#include "engine/components/health/health.h"
#include "engine/components/physics/physics.h"
#include "engine/components/sprite/sprite.h"
#include "engine/components/triggers/trigger.h"
#include "engine/engine.h"
#include "engine/entities/entity_manager.h"
#include "engine/render/render.h"
#include "engine/utils/util.h"
#include "linmath.h"
#include <stdio.h>
#include <stdlib.h>

Entity* create_entity(u32 id, Entity_Tag tag, string name){
    Entity* e = malloc(sizeof(Entity));
                       
    if(!e) {
        ERROR_RETURN(NULL, "Could not allocate for entity player\n");
    }
    
    *e = (Entity){
        .id = id,
        .tag = tag,
        .is_alive = true,
        .has_aabb = false,
        .has_sprite = false,
        .has_physics_body = false,
    };

    return e;
}

void update_viewing_direction(Entity* entity) {
    float vx = entity->physics_body->velocity[0];
    float vy = entity->physics_body->velocity[1];

    // If velocity is close to zero, don't update direction
    if (fabsf(vx) < 0.01f && fabsf(vy) < 0.01f) {
        return; // keep current direction
    }

    if (fabsf(vx) > fabsf(vy)) {
        if (vx > 0) {
            entity->viewing_direction = RIGHT;
        } else {
            entity->viewing_direction = LEFT;
        }
    } else {
        if (vy > 0) {
            entity->viewing_direction = DOWN;
        } else {
            entity->viewing_direction = UP;
        }
    }
}

void entity_update(Entity* entity) {
    if(!entity->is_alive) {
        return;
    }

    if(entity->has_physics_body && entity->has_aabb) {
        physics_body_update(entity->physics_body, entity->aabb);
        update_viewing_direction(entity);
    }

}

void render_entity(Entity* entity, u32 texture_slots[8]) {
    if(entity->has_aabb && entity->has_sprite) {
        render_sprite_sheet_frame(entity->sprite->sprite_sheet, entity->sprite->start_row, get_sprite_draw_col(entity->sprite, entity->physics_body->is_moving_left), entity->aabb->position, entity->viewing_direction, texture_slots);
        
        if(entity->has_health) {
            float index = 6;

            if(entity->health->current_health < 99) {
                index = 7;
            }
            
            if(entity->health->current_health < 50) {
                index = 8;
            }
            
            if(entity->health->current_health < 30) {
                index = 9;
            }

            vec2 render_position;
            vec2_add(render_position, entity->aabb->position, (vec2){0, 8});

            render_sprite_sheet_frame(entity->sprite->sprite_sheet, 1, index, render_position, NO_DIR, texture_slots);
        }

    }
}


Entity* create_player(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos) {
    Entity* e = create_entity(id, tag, "");
    
    e->has_aabb = true;
    AABB* aabb = malloc(sizeof(AABB)); 
    if(!aabb) {
        ERROR_RETURN(NULL, "Could not allocate for aabb on entity player\n");
    }
    *aabb = aabb_create(pos, size);
    e->aabb = aabb;

    e->has_physics_body = true;
    Physics_Body* body = malloc(sizeof(Physics_Body));

    if(!body) {
        ERROR_RETURN(NULL, "Could not allocate for physics body on entity player \n");
    }

    *body = physics_body_create();
    e->physics_body = body;

    e->has_sprite = true;
    Sprite* sprite = malloc(sizeof(Sprite));

    if(!sprite) {
        ERROR_RETURN(NULL, "Could not allocate for sprite on entity : player\n");
    }

    *sprite = sprite_init(sprite_sheet, sprite_pos[0], sprite_pos[1]);
    e->sprite = sprite;

    e->has_health = true;
    Health* health = malloc(sizeof(Sprite));

    if(!health) {
        ERROR_RETURN(NULL, "Could not allocate for health on entity : player\n");
    }

    *health = health_init(100);
    e->health = health;
    

    e->has_damage = true;
    Damage* damage = malloc(sizeof(Damage));

    if(!damage) {
        ERROR_RETURN(NULL, "Could not allocate for damage on entity : player\n");
    }

    *damage = damage_init(10, 0, false, 0, 100);
    e->damage = damage;

    return e;
}



Entity* create_enemy(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos) {
    Entity* e = create_entity(id, tag, "");
    
    e->has_aabb = true;
    AABB* aabb = malloc(sizeof(AABB)); 
    if(!aabb) {
        ERROR_RETURN(NULL, "Could not allocate for aabb on entity %u\n", id);
    }
    *aabb = aabb_create(pos, size);
    e->aabb = aabb;

    e->has_physics_body = true;
    Physics_Body* body = malloc(sizeof(Physics_Body));

    if(!body) {
        ERROR_RETURN(NULL, "Could not allocate for physics body on entity player %u\n", id);
    }

    *body = physics_body_create();
    e->physics_body = body;

    e->has_sprite = true;
    Sprite* sprite = malloc(sizeof(Sprite));

    if(!sprite) {
        ERROR_RETURN(NULL, "Could not allocate for sprite on entity %u\n", id);
    }

    *sprite = sprite_init(sprite_sheet, sprite_pos[0], sprite_pos[1]);
    e->sprite = sprite;

    e->has_health = true;
    Health* health = malloc(sizeof(Sprite));

    if(!health) {
        ERROR_RETURN(NULL, "Could not allocate for health on entity %u\n", id);
    }

    *health = health_init(100);
    e->health = health;
    

    e->has_damage = true;
    Damage* damage = malloc(sizeof(Damage));

    if(!damage) {
        ERROR_RETURN(NULL, "Could not allocate for damage on entity %u\n", id);
    }

    *damage = damage_init(10, 0, false, 0, 100);
    e->damage = damage;


    e->has_trigger_event = true;
    Trigger* trigger = malloc(sizeof(Trigger));

    if(!trigger) {
        ERROR_RETURN(NULL, "Could not allocate for trigger on entity %u\n", id);
    }

    *trigger = trigger_init(TRIGGER_DAMAGE, false);
    e->trigger = trigger;

    e->is_enemy = true;

    return e;
}
