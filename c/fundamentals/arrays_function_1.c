#include <stdio.h>

void f(int arr[]) {
    printf("sizeof(arr) = %zu\n", sizeof(arr));
}

int main(void) {
    int a[10];
    printf("sizeof(a)   = %zu\n", sizeof(a));
    f(a);
    return 0;
}