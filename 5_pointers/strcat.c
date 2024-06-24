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
 * Copy string t to the end of string s
 * String S must have enough size to include string t
 * Returns pointer to the resulting string, otherwise returns 0
 * C guarantees you cannot have a pointer with value 0
 */
char *strcat(char *dest, const char *src) {
    /*
     * Behavior is undefined:
     * - if s does not have enough space for both strings and NULL
     * - strings overlap
     * - if dest and src are not pointers to NULL terminated strings
     */
    int len = strlen(dest);
    for ( dest+=len; *src != '\0'; dest++, src++)
        *dest = *src;
    return dest;
}

int main(void) {
    char str1[50] = "hello ";
    char str2[] = "world";
    strcat(str1, str2);
    printf("%s\n", str1);
    return 0;
}
