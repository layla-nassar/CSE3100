#include <stdio.h>

// Function to calculate factorial
unsigned long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    unsigned long long fact = 1;
    for (int i = 2; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

// Function to calculate Catalan number
unsigned long long catalan(int n) {
    return factorial(2 * n) / (factorial(n + 1) * factorial(n));
}

int main() {
    int n;
    printf("Enter the value of n to calculate Catalan numbers: ");
    scanf("%d", &n);

    for (int i = 0; i <= n; i++) {
        printf("C(%d) = %llu\n", i, catalan(i));
    }

    return 0;
}