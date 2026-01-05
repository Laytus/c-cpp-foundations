#include <stdio.h>

int main(void) {
    int x = 42;

    int *p = &x;

    printf("x value: %d\n", x);
    printf("pointer p value: %p\n", (void *)p);

    return 0;
}