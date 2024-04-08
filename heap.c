//
// Created by mmiro on 14.10.2023.
//
#include "heap.h"
#include "custom_unistd.h"
#include <string.h>
#include "tested_declarations.h"
#include "rdebug.h"
#define fence_size 8
#define fence_type 'M'

struct memory_chunk_t{
    struct memory_chunk_t* prev;
    struct memory_chunk_t* next;
    size_t size;
    size_t sum_check;
} ;

struct memory_manager_t
{
    size_t size;
    void *memory_start;
    size_t memory_size;
    struct memory_chunk_t *head;
} list;

size_t validate_sum(struct memory_chunk_t *chunk){
    size_t res = 0;
    res += (size_t)chunk->next;
    res += (size_t)chunk->prev;
    res += chunk->size;
    return res;
}
int increase_heap(size_t pages){
    size_t new = pages / SIZE_OF_PAGE;
    new = new + 1;
    void *big = custom_sbrk((intptr_t)new * SIZE_OF_PAGE);
    if(big == (void *)-1){
        return -1;
    }
    list.memory_size += new*SIZE_OF_PAGE;
    return 0;
}

int heap_setup(void){
    void *memory = custom_sbrk(SIZE_OF_PAGE);
    if(memory == (void*)-1){
        return -1;
    }
    list.head = NULL;
    list.memory_size = SIZE_OF_PAGE;
    list.memory_start = memory;
    return 0;
}

void heap_clean(void){
    custom_sbrk((intptr_t)list.memory_size * (-1));
    list.memory_size = 0;
    list.head = NULL;
    list.memory_start = NULL;
}

void* heap_malloc(size_t size){
    if(size < 1){
        return NULL;
    }
    if(heap_validate()){
        return NULL;
    }
    struct memory_chunk_t *node = list.head;
    if(node != NULL && list.memory_start != node){
        size_t front = (char*)list.head - (char*)list.memory_start;
        if(front >= sizeof (struct memory_chunk_t) + fence_size + size + fence_size){
            struct memory_chunk_t *newchunk = list.memory_start;
            newchunk->prev = NULL;
            newchunk->next = list.head;
            newchunk->size = size;
            newchunk->sum_check = validate_sum(newchunk);
            list.head->prev = newchunk;
            list.head->sum_check = validate_sum(list.head);
            list.head = list.memory_start;
            char *move_front = (char*)newchunk + sizeof(struct memory_chunk_t);
            for (int i = 0; i < fence_size; ++i) {
                *(move_front + i) = fence_type;
            }
            move_front += newchunk->size + fence_size;
            for (int i = 0; i < fence_size; ++i) {
                *(move_front + i) = fence_type;
            }
            move_front -= newchunk->size;
            return move_front;
        }
    }
    if(node == NULL){
        if(sizeof (struct memory_chunk_t) + fence_size + size + fence_size > list.memory_size){
            if(increase_heap(sizeof (struct memory_chunk_t) + fence_size + size + fence_size) == -1){
                return NULL;
            }
        }
        node = list.memory_start;
        node->next = NULL;
        node->prev = NULL;
        node->size = size;
        node->sum_check = validate_sum(node);
        list.head = node;
        char *move = (char*)node + sizeof(struct memory_chunk_t);
        for (int i = 0; i < fence_size; ++i) {
            *(move + i) = fence_type;
        }
        move += node->size + fence_size;
        for (int i = 0; i < fence_size; ++i) {
            *(move + i) = fence_type;
        }
        move -= node->size;
        return move;
    }
    while(node->next != NULL){
        size_t place_bet = (char*)node->next - (char*)node;
        place_bet -= sizeof (struct memory_chunk_t) + fence_size + node->size + fence_size;
        if(place_bet >= size + sizeof (struct memory_chunk_t) + fence_size + fence_size){
            char *move_inside = (char*)node + sizeof (struct memory_chunk_t) + fence_size + node->size + fence_size;
            struct memory_chunk_t *newchunk = (struct memory_chunk_t *)move_inside;
            newchunk->next = node->next;
            newchunk->prev = node;
            newchunk->size = size;
            newchunk->sum_check = validate_sum(newchunk);
            node->next->prev = newchunk;
            node->next->sum_check = validate_sum(node->next);
            node->next = newchunk;
            node->sum_check = validate_sum(node);
            move_inside = (char*)newchunk + sizeof(struct memory_chunk_t);
            for (int i = 0; i < fence_size; ++i) {
                *(move_inside + i) = fence_type;
            }
            move_inside += fence_size + newchunk->size;
            for (int i = 0; i < fence_size; ++i) {
                *(move_inside + i) = fence_type;
            }
            move_inside -= newchunk->size;
            return move_inside;
        }
        node = node->next;
    }
    char *out_move = (char*)node + sizeof(struct memory_chunk_t) + fence_size + node->size + fence_size;
    if((size_t)(sizeof (struct memory_chunk_t) + fence_size + size + fence_size + out_move - (char*)list.memory_start) > list.memory_size){
        if(increase_heap(sizeof (struct memory_chunk_t) + fence_size + size + fence_size) == -1){
            return NULL;
        }
    }
    struct memory_chunk_t *newchunk = (struct memory_chunk_t*)out_move;
    newchunk->next = NULL;
    newchunk->prev = node;
    newchunk->size = size;
    newchunk->sum_check = validate_sum(newchunk);
    node->next = newchunk;
    node->sum_check = validate_sum(node);
    out_move += sizeof(struct memory_chunk_t);
    for (int i = 0; i < fence_size; ++i) {
        *(out_move + i) = fence_type;
    }
    out_move += fence_size + newchunk->size;
    for (int i = 0; i < fence_size; ++i) {
        *(out_move + i) = fence_type;
    }
    out_move -= newchunk->size;
    return out_move;
}


void* heap_calloc(size_t number, size_t size){
    char *mem = heap_malloc(number * size);
    if(mem == NULL){
        return NULL;
    }
    for (size_t i = 0; i < number * size; ++i) {
        *(mem + i) = 0;
    }
    return mem;
}


void* heap_realloc(void* memblock, size_t count){
    if((memblock != NULL) && (count == 0)){
        heap_free(memblock);
        return NULL;
    }
    if(memblock == NULL){
        return heap_malloc(count);
    }
    if(heap_validate()){
        return NULL;
    }
    
    int flag = 0;
    struct memory_chunk_t *node = list.head;

    while(node != NULL){
        if((void *)((char*)node + sizeof(struct memory_chunk_t) + fence_size) == memblock){
            flag = 1;
            break;
        }
        node = node->next;
    }

    if(flag){
        if(node->size >= count){
            node->size = count;
            if(node == NULL){
            }
            else{
                node->sum_check = validate_sum(node);
            }
            char *move_back = (char*)memblock + node->size;
            for (int i = 0; i < fence_size; ++i) {
                *(move_back + i) = fence_type;
            }

            return memblock;
        }

        if(node->next == NULL){
            if(increase_heap(count - node->size) != 0){
                return NULL;
            }
            node->size = count;
            if(node == NULL){
            }
            else{
                node->sum_check = validate_sum(node);
            }
            char *move_back = (char*)memblock + node->size;
            for (int i = 0; i < fence_size; ++i) {
                *(move_back + i) = fence_type;
            }


            return memblock;
        }

        if((((char*)node->next - (char*)node) - sizeof (struct memory_chunk_t) - fence_size- node->size - fence_size) >= count - node->size){
            node->size = count;
            if(node == NULL){
            }
            else{
                node->sum_check = validate_sum(node);
            }

            char *move_in = (char*)memblock + node->size;
            for (int i = 0; i < fence_size; ++i) {
                *(move_in + i) = fence_type;
            }


            return memblock;
        }



        char *newNode = heap_malloc(count);
        if(newNode == NULL){
            return NULL;
        }
        memcpy(newNode, memblock, node->size);
        heap_free(memblock);
        if(node == NULL){
        }
        else{
            node->sum_check = validate_sum(node);
        }

        return newNode;
    }
    else{
        return NULL;
    }
}
void  heap_free(void* memblock){
    if(memblock == NULL){
        return;
    }
    if(heap_validate()){
        return;
    }
    int flag = 0;
    struct memory_chunk_t *n = list.head;
    while(n != NULL){
        if(memblock == (char*)n + fence_size + sizeof (struct memory_chunk_t)){
            flag = 1;
        }
        n = n->next;
    }

    if(flag != 1){
          return;
    }
    n = (struct memory_chunk_t *) ((char *) memblock - fence_size - sizeof(struct memory_chunk_t));
    if(n->next == NULL && n->prev == NULL){
        list.head = NULL;
    }
    else if(n->next != NULL && n->prev == NULL){
        list.head = n->next;
        list.head->prev = NULL;
        list.head->sum_check = validate_sum(list.head);
    }
    else if(n->prev != NULL && n->next == NULL){
        n->prev->next = NULL;
        n->prev->sum_check = validate_sum(n->prev);
    }
    else if(n->next != NULL && n->prev != NULL){
        n->prev->next = n->next;
        n->prev->sum_check = validate_sum(n->prev);
        n->next->prev = n->prev;
        n->next->sum_check = validate_sum(n->next);
    }

}
int heap_validate(void){
    if(list.memory_start == NULL){
        return 2;
    }


    struct memory_chunk_t *check = list.head;

    while(check != NULL){
        char *move = (char*)check;
        if(validate_sum(check) != check->sum_check){
            return 3;
        }

        move += sizeof(struct memory_chunk_t);
        for (int i = 0; i < fence_size; ++i) {
            if(*(move + i) != fence_type){
                return 1;
            }
        }
        move += fence_size + check->size;
        for (int i = 0; i < fence_size; ++i) {
            if(*(move + i) != fence_type){
                return 1;
            }
        }
        check = check->next;
    }

    return 0;
}
enum pointer_type_t get_pointer_type(const void* const pointer){
    if(pointer == NULL){
        return pointer_null;
    }
    if(heap_validate()){
        return pointer_heap_corrupted;
    }
    struct memory_chunk_t *node = list.head;
    while(node != NULL){
        char *move = (char*)node;
        for (size_t i = 0; i < sizeof(struct memory_chunk_t); ++i) {
            if((char*) pointer == (move + i)){
                return pointer_control_block;
            }
        }
        move += sizeof(struct memory_chunk_t);
        for (int i = 0; i < fence_size; ++i) {
            if((char*) pointer == (move + i)){
                return pointer_inside_fences;
            }
        }
        move += fence_size;
        for (size_t i = 0; i < node->size; ++i) {
            if((char*) pointer == (move + i)){
                if(i == 0){
                    return pointer_valid;
                }
                else{
                    return pointer_inside_data_block;
                }
            }
        }
        move += node->size;
        for (int i = 0; i < fence_size; ++i) {
            if((char*) pointer == (move + i)){
                return pointer_inside_fences;
            }
        }

        node = node->next;
    }

    return pointer_unallocated;
}

size_t heap_get_largest_used_block_size(void){
    if(heap_validate()){
        return 0;
    }
    size_t largest = 0;
    struct memory_chunk_t *block = list.head;
    while(block != NULL){
        if(largest < block->size){
            largest = block->size;
        }
        block = block->next;
    }
    return largest;
}
