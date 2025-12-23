#include <stdio.h>

int main(void) {
    int x = 10;

    if (x > 0) {
        int y = 20;
        printf("Inside block: x = %d, y = %d\n", x, y);
    }

    printf("Outside block: x = %d\n", x);
    return 0;
}