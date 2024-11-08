#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc_header.h"

// Test: basic malloc and free
void test_malloc_free() {
    printf("Test: malloc and free\n");

    int *ptr = (int*) malloc(sizeof(int) * 10);
    if (ptr == NULL) {
        printf("malloc failed\n");
        return;
    }

    // Assign values to allocated memory
    for (int i = 0; i < 10; ++i) {
        ptr[i] = i;
    }

    // Check the values
    for (int i = 0; i < 10; ++i) {
        if (ptr[i] != i) {
            printf("Value mismatch at index %d: expected %d, got %d\n", i, i, ptr[i]);
        }
    }

    // Free the allocated memory
    free(ptr);
    printf("malloc and free passed\n");
}

// Test: realloc with larger size
void test_realloc_grow() {
    printf("Test: realloc to grow\n");

    int *ptr = (int*) malloc(sizeof(int) * 5);
    for (int i = 0; i < 5; ++i) {
        ptr[i] = i;
    }

    // Reallocate to a larger size
    ptr = (int*) realloc(ptr, sizeof(int) * 10);
    if (ptr == NULL) {
        printf("realloc failed\n");
        return;
    }

    // Verify the old values are intact and new space is zero
    for (int i = 0; i < 5; ++i) {
        if (ptr[i] != i) {
            printf("Value mismatch at index %d: expected %d, got %d\n", i, i, ptr[i]);
        }
    }

    for (int i = 6; i < 10; ++i) {
        if (ptr[i] != 0) {
            printf("Unexpected value at index %d\n", i);
        }
    }

    free(ptr);
    printf("realloc grow test passed\n");
}

// Test: realloc with smaller size
void test_realloc_shrink() {
    printf("Test: realloc to shrink\n");

    int *ptr = (int*) malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        ptr[i] = i;
    }

    // Reallocate to a smaller size
    ptr = (int*) realloc(ptr, sizeof(int) * 5);
    if (ptr == NULL) {
        printf("realloc failed\n");
        return;
    }

    // Verify the old values are intact
    for (int i = 0; i < 5; ++i) {
        if (ptr[i] != i) {
            printf("Value mismatch at index %d: expected %d, got %d\n", i, i, ptr[i]);
        }
    }

    free(ptr);
    printf("realloc shrink test passed\n");
}

// Test: malloc with 0 size
void test_malloc_zero_size() {
    printf("Test: malloc with size 0\n");

    void *ptr = malloc(0);
    if (ptr != NULL) {
        printf("malloc(0) should return NULL\n");
    }

    free(ptr);
    printf("malloc(0) passed\n");
}

// Test: free with NULL pointer
void test_free_null() {
    printf("Test: free NULL pointer\n");

    free(NULL);  // Should not crash
    printf("free(NULL) passed\n");
}

int main() {
    test_malloc_free();
    test_realloc_grow();
    test_realloc_shrink();
    test_malloc_zero_size();
    test_free_null();

    return 0;
}