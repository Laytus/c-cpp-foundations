#include "int_vector.h"
#include <stdio.h>

int main(void) {
    IntVector v;
    if (vec_init(&v, 4) != 0) return 1;

    for (int i = 0; i < 10; i++) {
        if (vec_push(&v, i * 10) != 0) {
            vec_destroy(&v);
            return 1;
        }
    }

    printf("size=%zu capacity=%zu\n", v.size, v.capacity);
    vec_destroy(&v);
    return 0;
}