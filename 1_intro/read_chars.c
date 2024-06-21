#include <stdio.h>

int main() {
    int c = 0;
    for (long counter = 0; (c = getchar()) != EOF; counter++) {
        printf("%ld: %c\n", counter, c);
    }
}
