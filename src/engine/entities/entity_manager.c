#include "entity_manager.h"
#include "engine/components/aabb/aabb.h"
#include "engine/components/damage/damage.h"
#include "engine/components/health/health.h"
#include "engine/components/physics/physics.h"
#include "engine/components/sprite/sprite.h"
#include "engine/components/triggers/trigger.h"
#include "engine/engine.h"
#include "engine/entities/entity.h"
#include "engine/utils/lists/array_list.h"
#include "engine/utils/util.h"
#include "linmath.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Entity_Manager_State entity_manager;

void entity_manager_init(void) {
    Array_List* entities = array_list_create(sizeof(Entity), 128);

    if(!entities) {
        free(entities);
        ERROR_EXIT("Could not create entity array_list!");
    }
    
    engine.entity_manager.entities = entities;
    engine.entity_manager.next_id = 0;
}

void do_tiggers(Entity* e, Entity* other) {
    if(can_event(other->trigger)) {
        switch(other->trigger->type) {
            case TRIGGER_SPRITE_CHANGE:
                set_sprite_draw_col(e->sprite, 1); 
                break;

            case TRIGGER_DAMAGE:
                if(e->health && other->damage) {
                    do_damage(e->health, other->damage);
                } 

                break;
        }

        other->trigger->is_triggered = true;
    }
}

void do_player_tiggers(Entity* player, Entity* other, Trigger_Type type) {
    if(player->tag != ENTITY_PLAYER) return;

    if(type == TRIGGER_DAMAGE && other->has_health) {
        do_damage(other->health, player->damage);
    }
}

bool is_within_range(vec2 a_pos, vec2 b_pos, float range) {
    float dx = a_pos[0] - b_pos[0];
    float dy = a_pos[1] - a_pos[1];

    float distance = pow(dx, 2) - pow(dy, 2);
    float range_sqrd = pow(range, 2);

    return distance <= range_sqrd;
}

void check_entity_collision(Entity* e) {
    if(engine.entity_manager.entities == NULL) {
        return;
    }

    for(size_t j = 0; j < engine.entity_manager.entities->length; ++j) {
        Entity* other = array_list_get(engine.entity_manager.entities, j);

        if(other->id == e->id) continue;

        if (!other || !other->has_aabb || !e->has_aabb || !e->aabb || !other->aabb) {
            continue;
        }

        if (is_boxes_colliding(e->aabb, other->aabb)) {
            if(e->tag == ENTITY_PLAYER && other->has_trigger_event && other->trigger != NULL) {
                do_tiggers(e, other);
            } else if(!other->has_physics_body && e->has_physics_body) {
                AABB* a = e->aabb;
                AABB* b = other->aabb;

                float dx = a->position[0] - b->position[0];
                float px = (a->half_size[0] + b->half_size[0]) - fabsf(dx);

                float dy = a->position[1] - b->position[1];
                float py = (a->half_size[1] + b->half_size[1]) - fabsf(dy);

                // No overlap
                if (px <= 0.0f || py <= 0.0f) return;

                // Resolve using the axis of least penetration
                if (px < py) {
                    float correction = (dx < 0) ? -px : px;
                    a->position[0] += correction;
                    e->physics_body->velocity[0] = 0.0f;
                } else {
                    float correction = (dy < 0) ? -py : py;
                    a->position[1] += correction;
                    e->physics_body->velocity[1] = 0.0f;
                }
            }
        }

        if(e->tag == ENTITY_PLAYER && is_within_range(e->aabb->position, other->aabb->position, 64)) {
            if(engine.input.attack) {
                do_player_tiggers(e, other, TRIGGER_DAMAGE);
            }
        }
    }

}

void update_entities(u32 texture_slots[8]) {
    if(engine.entity_manager.entities == NULL) {
        return;
    }

    for(size_t i = 0; i < engine.entity_manager.entities->length; ++i) {
        Entity* e = array_list_get(engine.entity_manager.entities, i);

        if(e->has_health && e->health->current_health <= 0) {
            e->is_alive = false;
        }
        
        if(!e->is_alive) {
            continue;
        }
        
        entity_update(e);
        check_entity_collision(e);
        render_entity(e, texture_slots);
    }
}

void destroy_entities(void) {
    for(size_t i = 0; i < engine.entity_manager.entities->length; ++i) {
        Entity* e = array_list_get(engine.entity_manager.entities, i);

        if(!e->is_alive && e->tag != ENTITY_PLAYER){
            if(e->has_aabb) {
                free(e->aabb);
            }

            if(e->has_physics_body) {
                free(e->physics_body);
            }

            if(e->has_health) {
                free(e->health);
            }
            

            if(e->has_damage) {
                free(e->damage);
            }
            
            array_list_remove(engine.entity_manager.entities, i);
        }
    } 
}

Entity* entity_get_by_id(u32 id) {
    if (!engine.entity_manager.entities) {
        ERROR_EXIT("Entity manager not initialized!");
    }

    for (size_t i = 0; i < engine.entity_manager.entities->length; ++i) {
        Entity* e = array_list_get(engine.entity_manager.entities, i);
        
        if (e->id == id){
            return e;
        }
    }
    
    return NULL;
}

Entity* entity_get_by_tag(Entity_Tag tag) {
    for (size_t i = 0; i < entity_manager.entities->length; ++i) {
        Entity* e = array_list_get(entity_manager.entities, i);
        
        if (e->tag == tag){
            return e;
        }
    }

    return NULL;
}


Entity* entity_create_player(u32 id, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos) {
    Entity* e = create_player(engine.entity_manager.next_id, ENTITY_PLAYER, pos, size, sprite_sheet, sprite_pos);

    array_list_append(engine.entity_manager.entities, e);
    engine.entity_manager.next_id++;

    return e;
}

Entity* entity_create_enemy(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos) {
    Entity* e = create_enemy(engine.entity_manager.next_id, tag, pos, size, sprite_sheet, sprite_pos);

    array_list_append(engine.entity_manager.entities, e);
    engine.entity_manager.next_id++;

    return e;
}
/*
Entity* entity_create_wall(u32 id, Entity_Tag tag, vec2 pos, vec2 size) {
    Entity* e = create_wall(engine.entity_manager.next_id, tag, pos, size);

    array_list_append(engine.entity_manager.entities, e);
    engine.entity_manager.next_id++;

    return e;
}

Entity* entity_create_door(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos) {
    Entity* e = create_player(engine.entity_manager.next_id, tag, pos, size, sprite_sheet, sprite_pos);

    array_list_append(engine.entity_manager.entities, e);
    engine.entity_manager.next_id++;

    return e;
} */
