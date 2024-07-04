#include <stdio.h>

#define VALUE -2545
#define HEX 16
#define BINARY 2

int main(void) {
    char str[1000];

    // convert to 16 base
    itob(VALUE, str, HEX);
    printf("Hex of %d: %s\n", VALUE, str);
    // binary
    itob(VALUE, str, BINARY);
    printf("Binary of %d: %s\n", VALUE, str);
}
