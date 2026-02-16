#include "int_vector.h"
#include <stdlib.h> // malloc, realloc, free

static int vec_is_valid(const IntVector *v) {
    if (!v) return 0;
    if ((v->data == NULL) != (v->capacity == 0)) return 0;
    if (v->size > v->capacity) return 0;
    return 1;
}

int vec_init(IntVector *v, size_t initial_capacity) {
    if (!v) return -1;

    v->data = NULL;
    v->size = 0;
    v->capacity = 0;

    if (initial_capacity == 0) return 0;

    v->data = (int *)malloc(initial_capacity * sizeof(int));
    if (!v->data) return -1;

    v->capacity = initial_capacity;
    return vec_is_valid(v) ? 0 : -1;
}

void vec_destroy(IntVector *v) {
    if (!v) return;
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

int vec_reserve(IntVector *v, size_t new_capacity) {
    if (!v || !vec_is_valid(v)) return -1;
    if (new_capacity <= v->capacity) return 0;

    void *tmp = realloc(v->data, new_capacity * sizeof(int));
    if (!tmp) return -1;

    v->data = (int *)tmp;
    v->capacity = new_capacity;
    return vec_is_valid(v) ? 0 : -1;
}

int vec_push(IntVector *v, int value) {
    if (!v || !vec_is_valid(v)) return -1;

    if (v->size == v->capacity) {
        size_t new_cap = (v->capacity == 0) ? 1 : (v->capacity * 2);
        if (vec_reserve(v, new_cap) != 0) return -1;
    }

    v->data[v->size++] = value;
    return vec_is_valid(v) ? 0 : -1;
}