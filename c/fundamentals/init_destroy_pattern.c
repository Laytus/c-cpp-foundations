// init_destroy_pattern.c
// Exercise: Init/Destroy patterns with a heap-owning struct (IntVector)
//
// Build + run (recommended)
//   clang -std=c17 -Wall -Wextra -Werror -O0 -g -fsanitize=address, undefined init_destroy_pattern.c -o vec
//   ./vec
//
// Goal:
// - Practice init/destroy patterns
// - Practice ownership + invariants
// - Practice realloc-safe growth
//
// Invariants (must always hold when vector is "valid")
// 1) data == NULL <=> capacity == 0
// 2) size <= capacity
// 3) if data != NULL then it points to a heap allocation of capacity * sizeof(int)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntVector;


// static -> functions only visible inside this .c file
static int vec_init(IntVector *v, size_t initial_capacity);
static void vec_destroy(IntVector *v);
static int vec_push(IntVector *v, int value);
static int vec_reserve(IntVector *v, size_t new_capacity);
static void vec_print(const IntVector *v, const char *label);

// Optional: debug helper to validate invariants at runtime while learning.
static int vec_is_valid(const IntVector *v);

int main(void) {
    IntVector v;

    // EXERCISE STEP 1:
    // Initialize with an initial capacity of 4.
    if (vec_init(&v, 4) != 0) {
        fprintf(stderr, "vec_init failed\n");
        return 1;
    }

    vec_print(&v, "v after init");

    // EXERCISE STEP 2:
    // Push 10 values (forces growth).
    for (int i = 0; i < 10; i++) {
        if (vec_push(&v, i * 10) != 0) {
            fprintf(stderr, "vec_push failed at i=%d\n", i);
            vec_destroy(&v);
            return 1;
        }
    }

    vec_print(&v, "v after pushes");
    
    // EXERCISE STEP 3:
    // Reserve capacity to at least 32 (should not change size).
    if (vec_reserve(&v, 32) != 0) {
        fprintf(stderr, "vec_reserve failed\n");
        vec_destroy(&v);
        return 1;
    }
    
    vec_print(&v, "v after reserve(32)");

    // EXERCISE STEP 4:
    // Destroy and ensure calling destroy twice is safe (should be no-op).
    vec_destroy(&v);
    vec_destroy(&v);

    puts("OK (no leaks, no UAF, no UB detected).");
    return 0;
}

static int vec_init(IntVector *v, size_t initial_capacity) {
    if (!v) return -1;

    // Always start from a known state (important for destroy safety).
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;

    if (initial_capacity == 0) {
        // Valid empty vector: data=NULL, capacity=0.
        return 0;
    }

    v->data = (int *)malloc(initial_capacity * sizeof(int));
    if (!v->data) {
        // Leave v in a valid "empty" state.
        return -1;
    }

    v->capacity = initial_capacity;
    // size already 0

    return vec_is_valid(v) ? 0 : -1;
}

static void vec_destroy(IntVector *v) {
    if (!v) return;

    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

static int vec_push(IntVector *v, int value) {
    if (!v || !vec_is_valid(v)) return -1;

    if (v->size == v->capacity) {
        // Grow: typical strategy = doublecapacity (or start at 1)
        size_t new_cap = (v->capacity == 0) ? 1 : (v->capacity * 2);

        // realloc-safe pattern (never overwrite v->data directly)
        if (vec_reserve(v, new_cap) != 0) {
            return -1;
        }
    }

    v->data[v->size] = value;
    v->size++;

    return vec_is_valid(v) ? 0 : -1;
}

static int vec_reserve(IntVector *v, size_t new_capacity) {
    if (!v || !vec_is_valid(v)) return -1;

    if (new_capacity <= v->capacity) {
        return 0;  // nothing to do
    }

    // realloc-safe pattern
    void *tmp = realloc(v->data, new_capacity * sizeof(int));
    if (!tmp) {
        // v->data remains valid and owned by v
        return -1;
    }

    v->data = (int *)tmp;
    v->capacity = new_capacity;

    return vec_is_valid(v) ? 0 : -1;
}

static void vec_print(const IntVector *v, const char *label) {
    if (!v) return;
    printf("%s: size=%zu capacity=%zu data=%p\n",
        label ? label : "vec",
        v->size, v->capacity, (void *)v->data
    );

    printf("  [");
    for (size_t i = 0; i < v->size; i++) {
        printf("%s%d", (i == 0 ? "" : ", "), v->data[i]);
    }
    printf("]\n");
}

static int vec_is_valid(const IntVector *v) {
    // Encoding invariants explicitly and checking them.
    if (!v) return 0;

    // 0 == false
    // any no-zero value == true

    // Invariants 1: data==NULL <=> capacity==0
    // data == NULL must be equivalent to capacity == 0.
    if ((v->data == NULL) != (v->capacity == 0)) return 0;

    // Invariants 2: size <= capacity
    // size larger than capacity is not valid
    if (v->size > v->capacity) return 0;

    return 1;
}