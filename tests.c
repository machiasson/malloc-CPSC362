#include "malloc_header.h"
#include <stdio.h>
#include <string.h>

void test_malloc_free() {
    void* ptr1 = malloc(50);
    if (ptr1 == NULL) {
        printf("Test malloc failed\n");
        return;
    }
    memset(ptr1, 1, 50); // Initialize memory
    free(ptr1);
    printf("Test malloc and free passed\n");
}

void test_realloc() {
    void* ptr1 = malloc(50);
    memset(ptr1, 1, 50); // Initialize memory
    ptr1 = realloc(ptr1, 100); // Resize
    if (ptr1 == NULL) {
        printf("Test realloc failed\n");
        return;
    }
    memset(ptr1, 2, 100); // Initialize resized memory
    free(ptr1);
    printf("Test realloc passed\n");
}

int main() {
    test_malloc_free();
    test_realloc();
    return 0;
}
