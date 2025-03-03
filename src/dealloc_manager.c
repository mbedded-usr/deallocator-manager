#include "dealloc_manager.h"
#include <stdbool.h>

dealloc_node* main_node = 0; // head of list

static void add_deallocator(deallocator dealloc_func)
{
    dealloc_node* new_list = (dealloc_node*)malloc(sizeof(dealloc_node));
    if (!main_node)
    {
        main_node = new_list;
        main_node->priv = 0;
        main_node->next = 0;
        main_node->last = main_node;
        main_node->dealloc_func = dealloc_func;
    }
    else
    {
        new_list->priv = main_node->last;
        main_node->last->next = new_list;
        main_node->last = new_list;
        new_list->dealloc_func = dealloc_func;
        new_list->next = 0;
        new_list->last = 0;
    }
}

static void insert_dealloc_func(dealloc_register* dealloc_info)
{
    if (!dealloc_info->is_deallocator_registered)
    {
        add_deallocator(dealloc_info->dealloc_func);
        dealloc_info->is_deallocator_registered = true;
    }
}

void* Malloc(int alloc_size, dealloc_register* dealloc_info)
{
    insert_dealloc_func(dealloc_info);
    void* alloc_memory = (void*)malloc(alloc_size);
    return alloc_memory;
}

void free_up()
{
    while (main_node != 0)
    {
        main_node->dealloc_func();
        dealloc_node* tmp = main_node;
        main_node = main_node->next;
        free(tmp);
    }
}
