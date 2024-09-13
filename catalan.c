#include <stdio.h>

// Recursive function to calculate n-th Catalan number using recurrence
long catalan_number(int n) {
    if (n == 0) {
        return 1;  // Base case: C(0) = 1
    } else {
        return (4 * n - 2) * catalan_number(n - 1) / (n + 1);  // Recurrence relation
    }
}

int main(void) {
    int n;

    while (scanf("%d", &n) == 1) {
        if (n < 0) {
            printf("C(%d) is not defined.\n", n);
        } else {
            printf("C(%d)=%18ld\n", n, catalan_number(n));
        }
    }

    return 0;
}