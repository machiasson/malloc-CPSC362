#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h> 

typedef struct block {
    size_t size;      
    struct block* next; 
    int free;           
} block_t;

block_t* find_block(size_t size);
void* get_block(size_t size);
void split_block(block_t* block, size_t size);
void combine_blocks(block_t* block);
void free(void* ptr);
void* realloc(void* ptr, size_t size);
void* malloc(size_t size);

#endif