#include <stdio.h>
#include <time.h>

#define SIZE 10
#define FIND 13

int binary_search1(int x, int v[], int size);
int binary_search2(int x, int v[], int size);

int main(void) {
    int numarray[SIZE] = {2, 3, 4, 5, 6, 10, 11, 12, 13, 14};
    int position = 0;

    // time task 1
    clock_t begin = clock();
    position = binary_search1(FIND, numarray, SIZE);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
    printf("Task 1: Found %d @ %d within %.8f\n", FIND, position, time_spent);

    // time task 2
    begin = clock();
    position = binary_search2(FIND, numarray, SIZE);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
    printf("Task 2: Found %d @ %d within %.8f\n", FIND, position, time_spent);
}

// textbook example of binary search, inefficient
int binary_search1(int x, int v[], int size) {
    int low, mid, high;
    low = 0;
    high = size - 1;

    while (low <= high) {
        mid = (low+high)/2;
        if (x < v[mid]) {
            high = mid - 1;
        } else if (x > v[mid]) {
            low = mid + 1;
        } else {
            return mid;
        }
    }
    return -1; // nothing found
}

// my own implementation
int binary_search2(int x, int v[], int size) {
    int low, mid, high = 0;
    high = size - 1;

    while (low <= high) {
        mid = low + (high-low)/2;
        if (x < v[mid]) {
            high = mid - 1;
        } else if (x > v[mid]) {
            low = mid + 1;
        } else {
            return mid;
        }
    }
    return -1; // nothing found
}
