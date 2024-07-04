#include <stdio.h>

#define LOWER_BOUND 0
#define UPPER_BOUND 300
#define STEP_SIZE 20

int main() {
    // calculate temperature
    for (int fahr = LOWER_BOUND; fahr <= UPPER_BOUND; fahr += STEP_SIZE) {
        printf("%dF is %.2fC\n", fahr, 5.0*(fahr-32)/9);
    }
}
