#include <stdio.h>

int main() {
    // declare variables
    float fahr, celcius = 0;
    int lower_bound = 0;
    int upper_bound = 300;
    int step_size = 20;

    // calculate temperature
    for (fahr = lower_bound; fahr <= upper_bound; fahr += step_size) {
        celcius = 5*(fahr-32)/9;
        printf("%.2fF is %.2f Celcius\n", fahr, celcius);
    }
}
