#include <stdio.h>

#define MAXLEN 1000

char line[MAXLEN];
void read_chars(void);

int main() {
    // extern variables
    extern char line[];
    read_chars();
    printf("%s", line);
}

void read_chars(void) {
    int i,c;
    extern char line[];
    for (i = 0; (c=getchar())!=EOF && c!='\n' && i<(MAXLEN-1); ++i) {
        line[i] = c;
    }
    // if end of line received
    if (c == '\n') {
        line[i] = c;
        line[i+1] = '\0';
    }
}
