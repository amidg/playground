//#include <stdio.h>
//#include <stdlib.h>
/*
 * works on macOS according to Apple Docs
 * also provides stuff like open, read, etc
 * https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/open.2.html
 */
#include <fcntl.h>
#include "syscalls.h"
#define PERMS 0660

int main(int argc, char* argv[]) {
    // check for arguments and read file
    char* prog = argv[0]; // program name
    FILE* file;
    char* error_msg = "error";
    char* hello_msg = "hello world";

    putchar('A');

    //for ( ; *hello_msg != '\0'; hello_msg++)
    //    putchar(*hello_msg);

    // work with the file provided
    if ((file = fopen(*++argv,"a")) == NULL) {
        return 1;
    }

    // if file opens OK
    return fseek(file, 3, SEEK_END);
}
