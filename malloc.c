// memory_manager.c
#include "malloc_header.h"
#include <unistd.h>    // For sbrk
#include <stdio.h>     // For fprintf (error checking)
#include <string.h>    // For memset (to initialize memory)

static block_t* free_list = NULL; // Head of the free list

// Helper function to get a large block of memory from the OS
void* get_me_blocks(size_t size) {
    block_t* block = sbrk(size);
    if (block == (void*) -1) {
        fprintf(stderr, "Error: sbrk failed\n");
        return NULL;
    }
    block->size = size;
    block->next = NULL;
    block->free = 1; // Mark as free
    return block;
}

// Find a free block that is large enough to satisfy the allocation
block_t* find_free_block(size_t size) {
    block_t* curr = free_list;
    while (curr != NULL) {
        if (curr->free && curr->size >= size) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// Split a large block into two blocks if it's too large
void split_block(block_t* block, size_t size) {
    if (block->size > size + sizeof(block_t)) {
        block_t* new_block = (block_t*) ((char*) block + size + sizeof(block_t));
        new_block->size = block->size - size - sizeof(block_t);
        new_block->next = block->next;
        new_block->free = 1;

        block->size = size;
        block->next = new_block;
    }
}

// Allocate a new block
void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Align size to be multiple of 8 bytes
    size = (size + 7) & ~7;

    block_t* block = find_free_block(size);
    if (block != NULL) {
        // Found a free block, use it
        block->free = 0;
        split_block(block, size);
        return (char*) block + sizeof(block_t);
    }

    // No suitable block found, request a new one from the OS
    block = get_me_blocks(size + sizeof(block_t));
    if (block == NULL) {
        return NULL; // Failed to allocate memory
    }
    block->free = 0;
    return (char*) block + sizeof(block_t);
}

// Coalesce adjacent free blocks into one larger block
void coalesce_free_blocks(block_t* block) {
    block_t* next = block->next;
    if (next != NULL && next->free) {
        block->size += sizeof(block_t) + next->size;
        block->next = next->next;
    }
}

// Free a block of memory
void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    block_t* block = (block_t*) ((char*) ptr - sizeof(block_t));
    block->free = 1;

    // Coalesce adjacent free blocks
    coalesce_free_blocks(block);

    // Add to the free list
    block->next = free_list;
    free_list = block;
}

// Reallocate memory
void* realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return malloc(size);
    }

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    block_t* block = (block_t*) ((char*) ptr - sizeof(block_t));
    if (block->size >= size) {
        split_block(block, size);
        return ptr;
    }

    // If the current block is too small, allocate a new one and copy the data
    void* new_ptr = malloc(size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, ptr, block->size);
        free(ptr);
    }
    return new_ptr;
}
