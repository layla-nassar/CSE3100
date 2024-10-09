#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Declare the firstDigit function
unsigned firstDigit(unsigned k)
{
    while (k >= 10)
        k /= 10;
    return k;
}

void FirstDigitCount(unsigned *a, unsigned n, unsigned freq[10])
{
    for (unsigned i = 0; i < 10; i++)
        freq[i] = 0;

    for (unsigned i = 0; i < n; i++) {
        unsigned digit = firstDigit(a[i]);
        freq[digit]++;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <number of integers>\n", argv[0]);
        return 1;  
    }

    unsigned n = atoi(argv[1]);

    assert(n >= 1 && n <= 100000);

    unsigned a[n], i;
    unsigned sum = 0; 
    for (i = 0; i < n; i++)
    {
        a[i] = sum;
        sum += i + 1;
    }
    unsigned freq[10];
    FirstDigitCount(a, n, freq);
    for (i = 0; i < 10; i++)
        printf("%d\t%d\n", i, freq[i]);
    return 0;
}