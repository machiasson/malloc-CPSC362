// memory_manager.h
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>  // For size_t

// Block metadata
typedef struct block {
    size_t size;        // The size of the block
    struct block* next; // Pointer to the next block
    int free;           // 1 if the block is free, 0 if allocated
} block_t;

// Function prototypes
void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);

// Internal utility functions
void* get_me_blocks(size_t size);
void split_block(block_t* block, size_t size);
block_t* find_free_block(size_t size);
void coalesce_free_blocks(block_t* block);

#endif
