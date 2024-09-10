#include <stdio.h>    // Includes the standard input/output library for functions like printf and scanf
#include <stdlib.h>   // Includes the standard library for functions like rand() and srand()
#include <assert.h>   // Includes the assert library to check for valid conditions (used later for input validation)

// Function to calculate the probability that the sum of 3 dice is at least k
double cum_prob(int k, long trials) {
    long success = 0;  // Initialize a counter for successful outcomes (when sum >= k)
    
    // Loop to simulate rolling 3 dice for the number of trials specified
    for (long i = 0; i < trials; i++) {
        // Generate a random number between 1 and 6 for each of the 3 dice
        int die1 = rand() % 6 + 1;  // rand() generates a random integer, % 6 ensures it's between 0-5, +1 makes it 1-6
        int die2 = rand() % 6 + 1;  // Roll the second die, similar logic to die1
        int die3 = rand() % 6 + 1;  // Roll the third die, similar logic to die1

        // Calculate the sum of the three dice
        int sum = die1 + die2 + die3;  // Add the values of die1, die2, and die3

        // Check if the sum of the dice is greater than or equal to k
        if (sum >= k) {
            success++;  // If the condition is true, increment the success counter
        }
    }
    
    // Calculate and return the probability as the fraction of successful trials out of total trials
    return (double)success / trials;  // Cast success to double to get a floating-point result when dividing
}

int main() {
    long n = 10000000;  // Define the number of trials to be 10 million
    int k;  // Declare an integer variable k to hold the minimum sum we are interested in

    // Prompt the user to input the value of k
    printf("Enter k: ");  // Display a message to the user asking for input
    scanf("%d", &k);  // Read the integer value entered by the user and store it in variable k

    // Use assert to check that the entered value of k is within the valid range (3 <= k <= 18)
    // The minimum sum of 3 dice is 3 (1 + 1 + 1), and the maximum sum is 18 (6 + 6 + 6)
    assert(k >= 3 && k <= 18);  // If the condition fails, the program will terminate with an error message

    // Seed the random number generator with a fixed value (12345) for reproducibility
    srand(12345);  // This ensures that the random sequence is the same every time the program runs

    // Call the cum_prob function to calculate the probability and print the result
    // The result is formatted to 5 decimal places with %.5lf
    printf("P(sum of the 3 dice is at least %d) = %.5lf\n", k, cum_prob(k, n));  // Print the result of cum_prob
    
    return 0;  // Return 0 to indicate that the program has executed successfully
}
