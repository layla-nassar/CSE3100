#include <stdio.h>

int main() {
    int i, j, n;

    // Assume the smallest such n is no more than 1000000
    for (n = 1; n <= 1000000; n++) {
        // Use count to record the number of different ways of summing two positive cube numbers to n
        int count = 0;

        // Loop over all possible i and j to check if their cubes sum up to n
        for (i = 1; i <= 100; i++) {
            for (j = i; j <= 100; j++) {
                // Check if the sum of cubes is equal to n
                if (i * i * i + j * j * j == n) {
                    count++;  // Found one way
                }
                // If count reaches 2, we can stop since we've found two distinct ways
                if (count == 2) {
                    break;  // Early exit the inner loop
                }
            }
            if (count == 2) {
                break;  // Early exit the outer loop
            }
        }

        // If we found 2 distinct ways, this is the smallest n, so break the main loop
        if (count == 2) {
            break;
        }
    }

    // Do not change code below
    printf("%d is the solution:\n", n);
    for (i = 1; i <= 100; i++) {
        for (j = i; j <= 100; j++) {
            if (i * i * i + j * j * j == n) {
                printf("%d * %d * %d + %d * %d * %d = %d\n", i, i, i, j, j, j, n);
            }
        }
    }

    return 0;
}
