#ifndef DEALLOC_MANAGER_TYPES
#define DEALLOC_MANAGER_TYPES

typedef void (*deallocator)();
typedef struct dealloc_node
{
    struct dealloc_node* priv;
    struct dealloc_node* next;
    struct dealloc_node* last;
    deallocator dealloc_func;
} dealloc_node;

#endif
