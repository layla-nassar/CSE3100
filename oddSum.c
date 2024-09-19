#include <stdio.h>
#include <stdlib.h>

int oddSumHelp(int count, int bound, int value)
{
    
    if (value == 0 && count == 0) return 1;  
    if (value < 0 || count <= 0 || bound <= 0) return 0;  

    
    if (bound % 2 == 0) bound--;

    
    if (oddSumHelp(count - 1, bound - 2, value - bound)) {
        printf("%d ", bound);  
        return 1;  
    }

    
    return oddSumHelp(count, bound - 2, value);
}


void oddSum(int count, int bound, int value)
{
    if (value <= 0 || count <= 0 || bound <= 0) return;

    if (bound % 2 == 0) bound -= 1;

    if (!oddSumHelp(count, bound, value)) {
        printf("No solutions.\n");
    } else {
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if(argc != 4) return -1;

    int count = atoi(argv[1]);
    int bound = atoi(argv[2]);
    int value = atoi(argv[3]);

    oddSum(count, bound, value);
    return 0;
}