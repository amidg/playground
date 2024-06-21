#include <stdio.h>
int main(void) {
    int test1 = 5;
    printf("Original: %d | Bitwise NOT: %d\n", test1, ~test1);
    printf("Original: %d | <<2: %d\n", test1, test1<<2);
    printf("Original: %d | >>1: %d\n", test1, test1>>1);
}
