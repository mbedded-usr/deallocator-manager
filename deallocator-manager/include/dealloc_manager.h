#ifndef DEALLOC_MANAGER_H
#define DEALLOC_MANAGER_H

#include "dealloc_manager_types.h"
#include <stdbool.h>

/*
 * @brief: a `Mem_bundle` is required to conduct
   the automated memory management. Always call `new_mem_bundle()`
   to make a new one. Then provide it with your custom `deallocaor`
   which will be used to release allocated memory. Finally pass it to `Malloc()`
*/
typedef struct Mem_bundle
{
    bool is_deallocator_registered;
    void* data;
    deallocator dealloc_func;
} Mem_bundle; // also consider the case where the 'dealloc_func' has special implementation.

struct Mem_bundle_array
{
   Mem_bundle* array;
   int size;
   int capacity;
};

Mem_bundle new_mem_bundle();
void* Malloc(int alloc_size, Mem_bundle* dealloc_info);
void free_up();

#endif

