#include <stdio.h>

void foo(void) {
    int x = 10;
    printf("foo: x = %d\n", x);
}

void bar(void) {
    int y = 20;
    foo();
    printf("bar: y = %d\n", y);
}

int main(void) {
    int z = 30;
    bar();
    printf("main: z = %d\n", z);
    return 0;
}