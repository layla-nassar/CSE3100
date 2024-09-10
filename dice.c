#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Function to calculate the cumulative probability
double cum_prob(int k, long trials) {
    int successful_trials = 0;
    for (int i = 0; i < trials; i++) {
        int sum = 0;
        // Simulate tossing 3 dice
        for (int j = 0; j < 3; j++) {
            // Generate random number between 1 and 6 for each die
            int die_roll = rand() % 6 + 1;
            sum += die_roll;
        }
        // Check if the sum is at least k
        if (sum >= k) {
            successful_trials++;
        }
    }
    // Calculate the probability
    double probability = ((double) successful_trials) / ((double) trials);
    return probability;
}

int main() {
    long n = 10000000;
    int k;

    printf("Enter k: ");
    scanf("%d", &k);
    assert(k >= 3 && k <= 18);

    srand(12345);

    // Output the probability with exactly 5 decimal places
    printf("P(sum of the 3 dice is at least %d) = %.5lf\n", k, cum_prob(k, n));

    return 0;
}
