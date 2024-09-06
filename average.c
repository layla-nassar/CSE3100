#include <stdio.h>

int main(void) {
    double x;
    double total = 0.0;
    double average = 0.0;
    int count = 0;

    // Reading input and calculating running total and average
    while (scanf("%lf", &x) == 1) {
        total += x;
        count++;
        average = total / count;
        printf("Total=%.6lf Average=%.6lf\n", total, average);
    }

    return 0;
}