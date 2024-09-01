#include <stdio.h>
#include <limits.h> // Include to use INT_MAX

int main(void) {
    int i, n;
    long long sum; // Use long long to prevent overflow
    sum = 0;
    printf("Enter n:\n");
    scanf("%d", &n);
    i = 1;
    while (i <= n) {
        sum = sum + i;
        i = i + 1;
    }
    printf("Sum from 1 to %d = %lld\n", n, sum); // Use %lld for long long

    // Checking for overflow
    if (sum > INT_MAX) {
        printf("Overflow occurred for n = %d\n", n);
    } else {
        printf("The sum is correct for n = %d\n", n);
    }

    return 0;
}
