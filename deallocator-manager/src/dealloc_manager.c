#include <stdlib.h>
#include <stdbool.h>
#include <dealloc_manager.h>
  
/*
 * @brief: pass this compile-time macro in order to terminate the program, if necessary 
*/
#ifdef WARNING_ON
#include <stdio.h>
#endif

/*
 * @brief: the purpose of `Mem_bundle_array` is to keep track of allocated memory block
   that will be released automatically on the normal program termination
*/
static Mem_bundle_array _mem_bundle_array = {
	.array = 0,
   	.size = 0,
        .capacity = 0
};

static void mem_bundle_array_init(Mem_bundle* _new_mem_bundle)
{
   _mem_bundle_array.array = (Mem_bundle*)malloc(sizeof(Mem_bundle));
   _mem_bundle_array.capacity = 1;
   _mem_bundle_array.array[_mem_bundle_array.size++] = *_new_mem_bundle;
}

static inline void mem_bundle_array_push(Mem_bundle* _new_mem_bundle)
{
   _mem_bundle_array.array[_mem_bundle_array.size++] = *_new_mem_bundle;
}

static void add_mem_bundle(Mem_bundle* _new_mem_bundle)
{
   if (_mem_bundle_array.array == 0)
   {
      mem_bundle_array_init(_new_mem_bundle);
   }
   else
   {
      if (_mem_bundle_array.size < _mem_bundle_array.capacity)
      {
         mem_bundle_array_push(_new_mem_bundle);
      }
      else
      {
         _mem_bundle_array.array = (Mem_bundle*)realloc(_mem_bundle_array.array,
                 sizeof(Mem_bundle) * (_mem_bundle_array.capacity * 2));
         _mem_bundle_array.capacity *= 2;
         mem_bundle_array_push(_new_mem_bundle); 
      }
   }
}

/*
 * @brief: inserts a `Mem_bundle` in case it was not registered previously
 * @param `_new_mem_bundle`: a new `Mem_bundle` to register
*/
static void insert_mem_bundle(Mem_bundle* _new_mem_bundle)
{
    if (_new_mem_bundle->is_deallocator_registered == false)
    {
        add_mem_bundle(_new_mem_bundle);
        _new_mem_bundle->is_deallocator_registered = true;
    }
}

Mem_bundle new_mem_bundle()
{
    Mem_bundle _new_mem_bundle = { .data = 0, 
	    			   .dealloc_func = 0, 
				   .is_deallocator_registered = false
   				 };
    return _new_mem_bundle;
}

/*
 * @brief: a wrapper around malloc() in order that stores the address of
   dynamically allocated memory blocks which will be freed up
   on the noraml program termination
 * @param `alloc_size`: required allocation size
 * @param `_mem_bundle`: a memory bundle consisting required info and data
   for dynamic memory allocation
 * @return: a `void*` to the requested memory
*/
void* Malloc(int alloc_size, Mem_bundle* _mem_bundle)
{
    #ifdef WARNING_ON
    if (_mem_bundle->data != 0)
    {
       fprintf(stderr, "WARNING at function %s, file %s: calling malloc() multiple"  \
               " times on the same memory block leads to memory loss.\n"             \
               "Program terminated.\n", __func__, __FILE__);
       exit(1);
    }
    #endif
    if (_mem_bundle_array.array != 0)
    {
	   atexit(free_up);
    }
    _mem_bundle->data = malloc(alloc_size); 
    insert_mem_bundle(_mem_bundle);
    return _mem_bundle->data;
}

static void free_pointer_array(deallocator dealloc_func, void* pointer_array)
{
    #ifdef WARNING_ON
    if (dealloc_func == 0)
    {
       fprintf(stderr, "WARNING at function %s, file %s: deallocator not specified."  \
		       "\nProgram terminated.\n", __func__, __FILE__);
       exit(1);
    }
    #endif
    dealloc_func(pointer_array);
}

/*
 * @brief: this function will be called to release allocated memory
   on the normal program termination
*/
void free_up()
{
    for (int i = 0; i < _mem_bundle_array.size; ++i) 
    {
	free_pointer_array(_mem_bundle_array.array[i].dealloc_func,
                          _mem_bundle_array.array[i].data);
    }
    if (_mem_bundle_array.array != 0)
    {
    	free(_mem_bundle_array.array);
    	_mem_bundle_array.array = 0;
    	_mem_bundle_array.size = 0;
    	_mem_bundle_array.capacity = 0;
    }
}

