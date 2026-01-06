#include <stdio.h>

int main(void) {
    int x = 42;
    int *p = &x;

    printf("x value: %d\n", x);
    printf("*p value: %d\n", *p);

    return 0;
}