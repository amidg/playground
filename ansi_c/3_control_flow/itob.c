#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define VALUE -2545
#define HEX 16
#define BINARY 2

void reverse(char str[], char inverse[]);
void itob(int n, char str[], int b);

int main(void) {
    char str[1000];

    // convert to 16 base
    itob(VALUE, str, HEX);
    printf("Hex of %d: %s\n", VALUE, str);
    // binary
    itob(VALUE, str, BINARY);
    printf("Binary of %d: %s\n", VALUE, str);
}

/*
 * ANSI C does not provide string reverse functionality
 */
void reverse(char str[], char inverse[]) {
    // this implementation avoids dynamic array allocation using malloc
    int i, j;
    for (i=strlen(str)-1, j=0; i >= 0; --i, ++j) {
        inverse[j] = str[i];
    }
    inverse[j] = '\n';
    inverse[++j] = '\0';
}

/*
 * Conversion from decimal to any base uses divison by base until remainder is 0
 */
void itob(int n, char str[], int b) {
    char temp[1000]; // empty string
    int remainder = 0;
    int i;
    bool isNegative = n < 0;
    if (isNegative) n = -n; // invert if negative
    for (i = 0; n > 0; i++, n /= b) {
        remainder = n % b;
        if (remainder >= 0 && remainder <= 9) {
            temp[i] = 48 + remainder;
        } else if (remainder > 9 && remainder < 16) {
            temp[i] = 65 + (remainder - 10);
        }
    }
    if (isNegative) temp[i++] = '-';
    temp[i] = '\0';
    reverse(temp, str);
}
