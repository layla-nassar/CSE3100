#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{
    int **visited = malloc((2 * n + 1) * sizeof(int *));
    for (int i = 0; i < (2 * n + 1); i++) {
        visited[i] = calloc(2 * n + 1, sizeof(int));  
    }

    int x = 0, y = 0;  
    visited[n][n] = 1; 

    int total_visited = 1;  
    int steps = 0;

    while (x > -n && x < n && y > -n && y < n) 
    {
        int r = rand() % 4; 
        switch (r) {
            case 0: y++; break;  
            case 1: x++; break;  
            case 2: y--; break;  
            case 3: x--; break;  
        }

        
        if (x > -n && x < n && y > -n && y < n && !visited[x + n][y + n]) {
            visited[x + n][y + n] = 1; 
            total_visited++;
        }

        steps++;
    }

   
    for (int i = 0; i < (2 * n + 1); i++) {
        free(visited[i]);
    }
    free(visited);

   
    return (double)total_visited / ((2 * n - 1) * (2 * n - 1)); 
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

    
    for (int n = 1; n <= 64; n *= 2) {
        double sum = 0.0;
        for (int i = 0; i < trials; i++) {
            sum += two_d_random(n);
        }
        
        printf("%d %.3lf\n", n, sum / trials);
    }

    return 0;
}
