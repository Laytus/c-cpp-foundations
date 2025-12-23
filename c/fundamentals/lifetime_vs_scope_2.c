#include <stdio.h>

void f(void) {
    int a = 1;
    printf("Inside f: a = %d\n", a);
}

void g(void) {
    int b = 2;
    f();
    printf("Inside g: b = %d\n", b);
}

void main(void) {
    int c = 3;
    g();
    printf("Inside main: c = %d\n", c);
    return 0;
}