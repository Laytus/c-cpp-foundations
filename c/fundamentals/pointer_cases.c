#include <stdio.h>

int main(void) {
    int x = 42;
    int *p = &x;

    printf("----- Values -----\n");
    printf("x        = %d\n", x);
    printf("*p       = %d\n", *p);
    
    printf("\n----- Addresses -----\n");
    printf("&x       = %p\n", (void *)&x);
    printf("p        = %p\n", (void *)p);
    
    printf("\n----- Pointer itself -----\n");
    printf("&p       = %p\n", (void *)&p);

    return 0;
}