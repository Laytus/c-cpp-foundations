#ifndef INT_VECTOR_H
#define INT_VECTOR_H

#include <stddef.h>  // size_t

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntVector;

int vec_init(IntVector *v, size_t initial_capacity);
void vec_destroy(IntVector *v);

int vec_push(IntVector *v, int value);
int vec_reserve(IntVector *v, size_t new_capacity);

#endif