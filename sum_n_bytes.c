#include <stdio.h>
#include <stdlib.h>

// Implement the following function
// The function sum_n_bytes returns the sum of the values of the first n bytes in the memory block
// pointed by the pointer p
unsigned sum_n_bytes(unsigned *p, int n)
{
    unsigned sum = 0;
    unsigned char *byte_ptr = (unsigned char *)p; // cast p to byte pointer
    for (int i = 0; i < n; i++) 
    {
        sum += byte_ptr[i]; // accumulate the sum of the first n bytes
    }
    return sum;
}

// Note how we use command line arguments
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s n\n", argv[0]);
        exit(-1);
    }
    int n = atoi(argv[1]); // convert argument to integer
    // allocate memory for n unsigned integers
    unsigned *p = calloc(n, sizeof(unsigned));
    // Assign values to these unsigned integers in a certain pattern
    int i;
    for(i = 0; i < n; i++)
    {
        p[i] = 1 << (i % 32);
    }
    // print out the sum of the first n bytes
    printf("%d\n", sum_n_bytes(p, n));
    // do not forget to free p
    free(p);
    return 0;
}
