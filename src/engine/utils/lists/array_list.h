#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stddef.h>
#include <stdio.h>
#include "../types.h"

typedef struct array_list {
    size_t length;
    size_t capacity;
    size_t item_size;
    void* items;
} Array_List;

Array_List* array_list_create(size_t item_size, size_t initial_capacity);
size_t array_list_append(Array_List* array_list, void* item);
void* array_list_get(Array_List* array_list, size_t index);
u8 array_list_remove(Array_List* array_list, size_t index);

#endif
