#include <stdio.h>
#include <stdlib.h>

int oddSumHelp(int count, int bound, int value)
{
    // Base cases
    if (value == 0 && count == 0) return 1;
    if (value < 0 || count <= 0 || bound <= 0) return 0;

    // Skip even numbers
    if (bound % 2 == 0) bound--;

    // Recursive case: try to include bound, or skip it
    if (oddSumHelp(count - 1, bound - 2, value - bound))
        return 1;
    
    // Try excluding current bound
    return oddSumHelp(count, bound - 2, value);
}

//Do not change the code below
void oddSum(int count, int bound, int value)
{
    	if(value <= 0 || count <= 0 || bound <= 0) return;
    
    	if(bound % 2 == 0) bound -= 1;

    	if(!oddSumHelp(count, bound, value)) printf("No solutions.\n");
	else printf("\n");
}

int main(int argc, char *argv[])
{
	if(argc != 4) return -1;

	int count = atoi(argv[1]);
	int bound = atoi(argv[2]);
	int value = atoi(argv[3]);

	//oddSum(12,30,200);
	//oddSum(10,20,100);
	//oddSum(20,20,200);
	oddSum(count, bound, value);
	return 0;
}
