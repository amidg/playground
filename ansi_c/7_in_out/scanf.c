#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000 // TODO: any better solution

int minscanf(char*, ...);

int main(int argc, char* argv[]) {
    int day, year;
    char month[20];
    // apparently original scanf works only with spaces
    if (minscanf("%d %s %d", &day, month, &year) == 3)
        printf("%d / %s / %d\n", day, month, year);
    return 0;
}

int minscanf(char* fmt, ...) {
    int found = 0; // number of matched args
    char input[MAXLEN]; // better solution is to use malloc()
    char arg[MAXLEN]; // argument used during processing
    va_list args; // points to each unnamed arg
    int* dval;
    char* sval;
    char* inptr = &input[0];
    char* argptr = &arg[0];

    // read char
    for ( ; (*inptr = getchar()) != '\n'; inptr++);
    *++inptr = '\0';

    // Identify words according to the pattern
    va_start(args, fmt); // point to the first unnamed arg
    inptr = &input[0]; // set input at the start
    for ( ; *fmt; fmt++) {
        // if we encounter % we start counting 
        if (*fmt == '%') {
            // break string into smaller string
            (*inptr == ' ') ? inptr++ : inptr;
            for ( ; *inptr != ' ' && *inptr != '\0'; inptr++, argptr++)
                *argptr = *inptr;
            *++argptr = '\0';
            argptr = &arg[0]; // always start from scratch
            // depending on the data type, update variables
            switch (*++fmt) {
                case 's': // string
                    sval = va_arg(args,char*); // pointer to the func arg
                    for ( ; *argptr != '\0'; sval++, argptr++)
                        *sval = *argptr;
                    printf("%s\n", sval);
                    break;
                case 'd': // int
                    dval = va_arg(args, int*);
                    *dval = atoi(argptr);
                    break;
            }
            found++;
            argptr = &arg[0]; // always start from scratch
        }
    }

    // we return number of arguments we matched
    va_end(args); // clean up when done
    return found;
}
