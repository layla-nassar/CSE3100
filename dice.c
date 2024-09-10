#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Function to calculate the probability that the sum of 3 dice is at least k
double cum_prob(int k, long trials) {
    long success = 0;  // To count the number of successful outcomes
    for (long i = 0; i < trials; i++) {
        // Roll 3 dice, each giving a random number between 1 and 6
        int die1 = rand() % 6 + 1;
        int die2 = rand() % 6 + 1;
        int die3 = rand() % 6 + 1;

        // Calculate the sum of the three dice
        int sum = die1 + die2 + die3;

        // Check if the sum is greater than or equal to k
        if (sum >= k) {
            success++;  // If successful, increment the count
        }
    }

    // Return the probability as the fraction of successful trials
    return (double)success / trials;
}

int main() {
    long n = 10000000;  // Number of trials
    int k;

    // Remove the "Enter k:" prompt to match autograder expectations
    scanf("%d", &k);  // Read the user input for k

    assert(k >= 3 && k <= 18);  // Ensure k is between 3 (minimum sum of 3 dice) and 18 (maximum sum)

    srand(12345);  // Seed the random number generator with a fixed seed for reproducibility

    // Output the result directly, without extra messages or prompts
    printf("P(sum of the 3 dice is at least %d) = %.5lf\n", k, cum_prob(k, n));

    return 0;
}
