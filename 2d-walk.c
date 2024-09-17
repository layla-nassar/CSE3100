#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{
    // Dynamically allocate memory for the visited array
    int **visited = malloc((2 * n + 1) * sizeof(int *));
    for (int i = 0; i < (2 * n + 1); i++) {
        visited[i] = calloc(2 * n + 1, sizeof(int));  // Initialize all elements to 0
    }

    int x = 0, y = 0;
    visited[n][n] = 1; // Mark start position as visited

    int total_visited = 1;  // Start position is counted
    int steps = 0;          // Number of steps taken

    // Random walk logic
    while (x > -n && x < n && y > -n && y < n)
    {
        int r = rand() % 4; // Random direction

        // Move walker based on random number
        if (r == 0) y++;      // Up
        else if (r == 1) x++; // Right
        else if (r == 2) y--; // Down
        else x--;             // Left

        // Check if the new position is within bounds and not yet visited
        if (x >= -n && x <= n && y >= -n && y <= n && !visited[x + n][y + n])
        {
            visited[x + n][y + n] = 1; // Mark as visited
            total_visited++;
        }
        steps++;
    }

    // Free dynamically allocated memory
    for (int i = 0; i < (2 * n + 1); i++) {
        free(visited[i]);
    }
    free(visited);

    // Return the fraction of visited points
    return (double)total_visited / ((2 * n) * (2 * n));
}

//Do not change the code below
int main(int argc, char *argv[])
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}
