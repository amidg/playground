#include <stdio.h>

/*
 * Return string length
 */
int strlen(const char *s) {
    int size;
    for (size=0; *s++ != '\0'; size++);
    return size;
}

/*
 * Reverse string
 */
void reverse(char *str) {
    char temp;
    int ch;
    int len = strlen(str);
    for (int i = 0; i < len/2; i++) {
        temp = *(str+i);
        ch = len-1-i;
        *(str+i) = *(str+ch);
        *(str+ch) = temp;
    }
}

int main(void) {
    char str[] = "yellow";
    printf("%s\n", str);
    reverse(str);
    printf("%s\n", str);
    return 0;
}
