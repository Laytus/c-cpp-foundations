#include <stdio.h>

int main(void) {
    char s[3] = {'h', 'i', '!'};

    printf("s    = %s\n", s);
    printf("s[0] = %p\n", (void *)&s[0]);
    printf("s[1] = %p\n", (void *)&s[1]);
    printf("s[2] = %p\n", (void *)&s[2]);
 
    return 0;
}