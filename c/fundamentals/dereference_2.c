#include <stdio.h>

int main(void) {
    int x = 42;
    int *p = &x;

    printf("x value: %d\n", x);

    *p = 100;

    printf("x value: %d\n", x);
    return 0;
}