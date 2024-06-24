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
 * Return 1 if t occurs at the end of s or return 0 otherwise
 */
int strend(const char *s, const char *t) {
    int len2 = strlen(t);
    for ( s += strlen(s)-len2; *t != '\0'; s++, t++)
        if (*s != *t)
            return 0;
    return 1;
}

int main(void) {
    char str1[] = "hello world";
    char str2[] = "world";
    printf("%d\n", strend(str1, str2));
    return 0;
}
