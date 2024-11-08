#include "malloc_header.h"
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>

static block_t* free_list = NULL;

block_t* find_block(size_t size) {
    block_t* current_block = free_list;
    while (current_block != NULL) {
        if (current_block->free && current_block->size >= size) {
            return current_block;
        }
        current_block = current_block->next;
    }
    return NULL;
}

void* get_block(size_t size) {
    block_t* block = sbrk(size);
    if (block == (void*) -1) {
        fprintf(stderr, "Error: sbrk failed\n");
        return NULL;
    }
    block->size = size;
    block->next = NULL;
    block->free = 1;
    return block;
}

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

void combine_blocks(block_t* block) {
    block_t* next = block->next;
    if (next != NULL && next->free) {
        block->size += sizeof(block_t) + next->size;
        block->next = next->next;
    }
}

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    block_t* block = (block_t*) ((char*) ptr - sizeof(block_t));
    block->free = 1;
    combine_blocks(block);
    block->next = free_list;
    free_list = block;
}

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
    void* new_ptr = malloc(size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, ptr, block->size);
        free(ptr);
    }
    return new_ptr;
}

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    size = ((size + 7) / 8) * 8;
    block_t* block = find_block(size);
    if (block != NULL) {
        block->free = 0;
        split_block(block, size);
        return (char*) block + sizeof(block_t);
    }
    block = get_block(size + sizeof(block_t));
    if (block == NULL) {
        return NULL;
    }
    block->free = 0;
    return (char*) block + sizeof(block_t);
}
