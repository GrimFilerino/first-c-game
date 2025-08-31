#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "engine/components/aabb/aabb.h"
#include "engine/components/damage/damage.h"
#include "engine/components/health/health.h"
#include "engine/components/physics/physics.h"
#include "engine/components/sprite/sprite.h"
#include "engine/components/triggers/trigger.h"
#include "engine/utils/lists/array_list.h"
#include <stdbool.h>

#define MAX_ENTITIES 5000

typedef enum {
    ENTITY_NONE,
    ENTITY_PLAYER,
    ENTITY_OLD_MAN,
    ENTITY_CENTRY,
    ENTITY_GOBILN
} Entity_Tag;

typedef enum {
    NO_DIR,
    UP,
    DOWN,
    LEFT, 
    RIGHT
} Viewing_Direction;

typedef struct entity {
    u32 id;
    Entity_Tag tag;
    bool is_alive;
    Viewing_Direction viewing_direction;

    bool has_physics_body;
    Physics_Body* physics_body;

    bool has_aabb;
    AABB* aabb;

    bool has_sprite;
    Sprite* sprite;

    bool has_trigger_event;
    Trigger* trigger;

    bool has_damage;
    Damage* damage;

    bool has_health;
    Health* health;
    
    bool is_enemy; 

} Entity;

typedef struct entity_manager_state {
    Array_List* entities;
    u32 next_id;
} Entity_Manager_State;

extern Entity_Manager_State entity_manager;

void entity_manager_init(void);
u32 entity_create(Entity_Tag tag, string name);
void update_entities(u32 texture_slots[8]);
void destroy_entities(void);

Entity* entity_get_by_id(u32 id);
Entity* entity_get_by_tag(Entity_Tag tag);

Entity* entity_create_player(u32 id, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos); 
Entity* entity_create_enemy(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos);

#endif

