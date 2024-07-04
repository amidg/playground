#include <stdio.h>
#include <limits.h>

#define MAX_STRING_LENGTH 1000

// declarations
void read_chars(char out[], int limit); 
void reverse(char out[], char in[]);

// main function
int main() {
    char string[MAX_STRING_LENGTH];
    char reverse_string[MAX_STRING_LENGTH];
    read_chars(string, MAX_STRING_LENGTH);
    reverse(reverse_string, string);
    printf("%s", string);
    printf("%s", reverse_string);
}

// definitions
void read_chars(char out[], int limit) {
    int i,c;
    for (i = 0; (c=getchar())!=EOF && c!='\n' && i<(limit-1); ++i) {
        out[i] = c;
    }
    // if end of line received
    if (c == '\n') {
        out[i] = c;
        out[i+1] = '\0';
    }
}

void reverse(char out[], char in[]) {
    int i,j=0; // i is out[] index, j is in[] index
    // calculate string length
    while (in[j] != '\n') {
        j++;
    }

    // reverse
    while (j >= 0) {
        out[i] = in[j];
        i++;
        j--;
    }
    out[i] = '\n';
    out[i+1] = '\0';
}
