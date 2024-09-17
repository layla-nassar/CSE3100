#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{
    // Dynamically allocate memory for the visited array
    int **visited = malloc((2 * n + 1) * sizeof(int *));
    for (int i = 0; i < (2 * n + 1); i++) {
        visited[i] = calloc(2 * n + 1, sizeof(int));  // Initialize all elements to 0
    }

    int x = 0, y = 0;  // Start at the center
    visited[n][n] = 1; // Mark start position as visited

    int total_visited = 1;  // Start position is counted
    int steps = 0;

    while (x > -n && x < n && y > -n && y < n) // Stop once we reach boundary
    {
        int r = rand() % 4; // Random direction
        switch (r) {
            case 0: y++; break;  // Move up
            case 1: x++; break;  // Move right
            case 2: y--; break;  // Move down
            case 3: x--; break;  // Move left
        }

        // Check if we are within the bounds and if the position is not yet visited
        if (x > -n && x < n && y > -n && y < n && !visited[x + n][y + n]) {
            visited[x + n][y + n] = 1; // Mark position as visited
            total_visited++;
        }

        steps++;
    }

    // Free dynamically allocated memory
    for (int i = 0; i < (2 * n + 1); i++) {
        free(visited[i]);
    }
    free(visited);

    // Return the fraction of points visited inside the square (excluding the boundary)
    return (double)total_visited / ((2 * n - 1) * (2 * n - 1)); // Exclude boundary
}

int main(int argc, char *argv[])
{
    int trials = 1000;
    int seed;

    if (argc == 2)
        seed = atoi(argv[1]);
    else
        seed = 12345;

    srand(seed);

    // Run the random walk for different values of n (1, 2, 4, 8, 16, 32, 64)
    for (int n = 1; n <= 64; n *= 2) {
        double sum = 0.0;
        for (int i = 0; i < trials; i++) {
            sum += two_d_random(n);
        }
        // Print the average fraction of visited points for the current n
        printf("%d %.3lf\n", n, sum / trials);
    }

    return 0;
}
