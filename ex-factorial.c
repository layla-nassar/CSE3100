#include <stdio.h>

// Recursive function to calculate factorial
int factorial(int n) {
    if (n == 0) {
        return 1;  // Base case: factorial(0) = 1
    } else {
        return n * factorial(n - 1);  // Recursive call
    }
}

int main() {
    int n, result;

    if (scanf("%d", &n) != 1 || n < 0) {
        printf("Error: Please enter a non-negative integer.\n");
        return 1;  // Error if input is invalid
    }

    result = factorial(n);
    printf("%d\n", result);

    return 0;
}