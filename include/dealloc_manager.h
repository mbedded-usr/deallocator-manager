#include "dealloc_manager_types.h"
#include <stdbool.h>

#ifndef DEALLOC_MANAGER_H
#define DEALLOC_MANAGER_H

#define Return(ret_code) {                   \
                            free_up();       \
                            return ret_code; \
                         }

typedef struct dealloc_register
{
    bool is_deallocator_registered;
    deallocator dealloc_func;
} dealloc_register;

static void add_deallocator(deallocator dealloc_func);
static void insert_dealloc_func(dealloc_register* dealloc_info);
void* Malloc(int alloc_size, dealloc_register* dealloc_info);
void free_up();

#endif
