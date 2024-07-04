#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000 // TODO: any better solution

int main(int argc, char* argv[]) {
    // read command line arguments
    char* prog = argv[0]; // program name for errors
    FILE *file1, *file2; // files to open
    int diff_line = -1;
    char line1[MAXLEN];
    char line2[MAXLEN];
    
    /*
     * check how many files are provided
     * argc == 1 -> only program itself
     * argc == 2 -> program itself + 1 file, nothing to compare
     * argc >2 -> program itself + files to compare with
     */
    if (argc > 2) {
        // if file has read error
        if ((file1 = fopen(*++argv, "r")) == NULL ||
            (file2 = fopen(*++argv, "r")) == NULL) {
            fprintf(stderr, "%s: can't open %s\n", prog, *argv);
            exit(1);
        } else {
            // if files opens OK
            diff_line = 1;
            while (fgets(line1, MAXLEN, file1) != NULL && 
                    fgets(line2, MAXLEN, file2) != NULL) { // NULL -> end of file
                // get lines and compare them using strcmp
                if (strcmp(line1, line2) != 0) {
                    printf("%d\n", diff_line);
                    exit(0);
                }
                diff_line++;
            }

            // close files after done
            fclose(file1);
            fclose(file2);
        }
    }

    // if stdout has error
    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        exit(2);
    }
    exit(0); // requires stdlib.h
}
