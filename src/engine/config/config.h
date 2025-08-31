#ifndef CONFIG_H
#define CONFIG_H

#include "../input/input.h"
#include "../utils/types.h"

#define AMOUNT_OF_KEY_BINDS 7

typedef struct config {
    u8 key_binds[AMOUNT_OF_KEY_BINDS];
} Config_State;

void config_init();
void config_bind_key(Input_Type key, const char* key_name);

#endif
