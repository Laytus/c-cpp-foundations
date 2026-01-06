#include <stdio.h>

void change(int *x) {
    *x = 100;
}

int main(void) {
    int a = 10;
    printf("a = %d\n", a);
    change(&a);
    printf("a = %d\n", a);
    return 0;
}