//
// Created by mmiro on 14.10.2023.
//

#ifndef SO2ALOKATORXD_HEAP_H
#define SO2ALOKATORXD_HEAP_H
#include <stdio.h>


#define SIZE_OF_PAGE 4096
enum pointer_type_t
{
    pointer_null,
    pointer_heap_corrupted,
    pointer_control_block,
    pointer_inside_fences,
    pointer_inside_data_block,
    pointer_unallocated,
    pointer_valid
};


//main functions
int heap_setup(void);
void heap_clean(void);
void* heap_malloc(size_t size);
void* heap_calloc(size_t number, size_t size);
void* heap_realloc(void* memblock, size_t count);
void  heap_free(void* memblock);
int heap_validate(void);
enum pointer_type_t get_pointer_type(const void* const pointer);
size_t heap_get_largest_used_block_size(void);



#endif //SO2ALOKATORXD_HEAP_H
