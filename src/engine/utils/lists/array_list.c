#include "array_list.h"
#include "engine/utils/util.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LIST_INITIAL_CAPACITY 1

Array_List* array_list_create(size_t item_size, size_t initial_capacity) {
    if (item_size == 0) {
        ERROR_RETURN(NULL, "Invalid item size (0).\n");
    }
    
    if (initial_capacity != 0 && SIZE_MAX / item_size < initial_capacity) {
        ERROR_RETURN(NULL, "Integer overflow during memory allocation.\n");
    }
    
    Array_List *array_list = malloc(sizeof(Array_List));
    
    if(!array_list) {
        ERROR_RETURN(NULL, "Could not allocate enough memory for array_list.\n") 
    }

    array_list->item_size = item_size;
    array_list->capacity = initial_capacity;
    array_list->length = 0;
    array_list->items = malloc(item_size * initial_capacity);
    if(!array_list->items) {
        free(array_list); //Clean up array_list
        ERROR_RETURN(NULL, "Could not allocate enough memory for items in array_list.\n");
    }

    return array_list;
}

size_t array_list_append(Array_List* array_list, void* item) {
    if (!array_list || !item) {
        ERROR_RETURN(0, "Invalid arguments passed to array_list_append.\n");
    }

    if(array_list->length == array_list->capacity) { 
        if(array_list->capacity == 0){
            array_list->capacity = ARRAY_LIST_INITIAL_CAPACITY; 
        } else {
            array_list->capacity *=2;
        }

        //Check for int overflow
        if (SIZE_MAX / array_list->item_size < array_list->capacity) {
            ERROR_RETURN(0, "Integer overflow during memory allocation.\n");
        }

        void* items = realloc(array_list->items, array_list->item_size * array_list->capacity);
        
        if(!items) {
            ERROR_RETURN(0, "Could not allocate enough memory for the new item in the array_list.\n");
        }
        
        array_list->items = items;
    }

    size_t index = array_list->length++;
    memcpy((u8*)array_list->items + index * array_list->item_size, item, array_list->item_size);

    return index;
}

void* array_list_get(Array_List* array_list, size_t index) {
    if (!array_list) {
        ERROR_RETURN(NULL, "array_list is NULL.");
    }

    if(index >= array_list->length) {
        ERROR_RETURN(NULL, "Error index is out of bounds for this array_list.\n");
    }

    return (u8*)array_list->items + index * array_list->item_size;
}

u8 array_list_remove(Array_List* array_list, size_t index){
    if (!array_list) {
        ERROR_RETURN(1, "array_list is NULL.");
    }

    if(index >= array_list->length) {
        ERROR_RETURN(1, "Error index is out of bounds for this array_list.\n");
    }

    if(array_list->length == 0) {
        ERROR_RETURN(1, "List is empty. \n");
    }


    if (!array_list->items && array_list->length > 0) {
        ERROR_RETURN(1, "Corrupt array_list: items pointer is NULL.\n");
    }   

    if(array_list->length == 1) {
        array_list->length = 0;
        return 0;
    }

    --array_list->length;

    u8* item_ptr = (u8*)array_list->items + index * array_list->item_size;
    u8* end_ptr = (u8*)array_list->items + array_list->length * array_list->item_size;

    memcpy(item_ptr, end_ptr, array_list->item_size);

    return 0;
}
