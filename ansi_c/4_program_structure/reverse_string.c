#include <stdio.h>
#include <string.h>

void reverse(char str[]);
void simple(char str[]);
void swap(char str[], int start, int end);

int main(void) {
    char word[] = "yellow";
    printf("Original: %s\n", word);
    simple(word);
    printf("Simple Reverse: %s\n", word);
    reverse(word);
    printf("Recursive Reverse: %s\n", word);
}

void swap(char str[], int start, int end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
}

void simple(char str[]) {
    int len = strlen(str);
    for (int i = 0; i < len/2; i++) {
        swap(str, i, len-1-i);
    }
}

void reverse(char str[]) {
    int len = strlen(str);
    // we always swap when possible
    swap(str, 0, len-1);
    // if we can afford to keep iterating, we keep reversing
    if (len > 2) {
        char temp[strlen(str)-2];
        int i,j;
        for (i=0,j=1;i<strlen(temp)-1;i++,j++) temp[i] = str[j];
        reverse(temp);
        printf("%s\n", temp);
        //for (i=0,j=1;i<strlen(temp);i++,j++) str[j] = temp[i];
    }
}
