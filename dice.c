#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Function to calculate the probability that the sum of 3 dice is at least k
double cum_prob(int k, long trials) {
    long success = 0;  // To count the number of successful outcomes where sum >= k
    for (long i = 0; i < trials; i++) {
        // Generate random numbers between 1 and 6 for each die
        int die1 = rand() % 6 + 1;  // Roll the first die
        int die2 = rand() % 6 + 1;  // Roll the second die
        int die3 = rand() % 6 + 1;  // Roll the third die

        // Calculate the sum of the three dice
        int sum = die1 + die2 + die3;

        // Check if the sum is greater than or equal to k
        if (sum >= k) {
            success++;  // If the sum is >= k, increment the success count
        }
    }

    // Calculate the probability as the number of successes divided by the total trials
    return (double)success / trials;  // Return the probability as a double
}

int main() {
    long n = 10000000;  // Number of trials (10 million dice rolls)
    int k;

    // Read the input value of k without printing any prompts
    scanf("%d", &k);  // Read the user input for k

    // Ensure that k is between 3 (minimum sum of 3 dice) and 18 (maximum sum of 3 dice)
    assert(k >= 3 && k <= 18);

    // Seed the random number generator with a fixed value for reproducibility
    srand(12345);

    // Call cum_prob to calculate the probability and print the result with 5 decimal places
    printf("P(sum of the 3 dice is at least %d) = %.5lf\n", k, cum_prob(k, n));

    return 0;  // Return 0 to indicate successful execution
}
