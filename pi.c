#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;
    double pi = 0.0;
    double term, pow16;

    printf("n = ");
    scanf("%d", &n);

    pow16 = 1; // This represents 16^i, starting with 16^0 = 1
    for (i = 0; i <= n; i++) {
        // Calculate the current term in the summation
        term = (4.0 / (8 * i + 1) - 2.0 / (8 * i + 4) - 1.0 / (8 * i + 5) - 1.0 / (8 * i + 6)) / pow16;
        
        // Add the term to the approximation of pi
        pi += term;
        
        // Update pow16 for the next iteration (16^i)
        pow16 *= 16;
    }

    // Print the result
    printf("PI = %.10f\n", pi);
    return 0;
}
