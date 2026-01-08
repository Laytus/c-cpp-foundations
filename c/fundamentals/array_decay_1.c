#include <stdio.h>

int main(void) {
    int arr[4] = {10, 20, 30, 40};

    int *p = arr;

    printf("arr[0] = %d\n", arr[0]);
    printf("p      = %d\n", *p);

    return 0;
}