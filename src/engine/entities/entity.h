#ifndef ENTITY_H
#define ENTITY_H

#include "entity_manager.h"
#include "engine/utils/types.h"
#include <stdbool.h>
#include <stddef.h>


Entity* create_entity(u32 id, Entity_Tag tag, string name);
void entity_update(Entity* entity);
void render_entity(Entity* entity, u32 texture_slots[8]);

Entity* create_player(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos); 
Entity* create_enemy(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos);
Entity* create_wall(u32 id, Entity_Tag tag, vec2 pos, vec2 size);
Entity* create_door(u32 id, Entity_Tag tag, vec2 pos, vec2 size, Sprite_Sheet* sprite_sheet, vec2 sprite_pos);
#endif
