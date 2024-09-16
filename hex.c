#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//convert the decimal integer d to hexadecimal, the result is stored in hex[]
void dec_hex(int d, char hex[])
{
    char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
                     'C', 'D', 'E', 'F'};

    int k = 0;

    // Edge case: if the number is 0
    if (d == 0) {
        hex[k++] = '0';
    }
    
    // Convert decimal to hexadecimal
    while (d > 0) {
        int remainder = d % 16;  // Get the remainder when divided by 16
        hex[k++] = digits[remainder];  // Store the corresponding hex digit
        d /= 16;  // Reduce the number by dividing by 16
    }

    // Reverse the digits to get the correct hexadecimal representation
    for (int i = 0; i < k / 2; ++i) {
        char temp = hex[i];
        hex[i] = hex[k - i - 1];
        hex[k - i - 1] = temp;
    }

    //Make sure the last character is a zero so that we can print the string correctly
    hex[k] = '\0';
}

// Do not change the code below
int main()
{
    int d;
    char hex[80];
    
    printf("Enter a positive integer : ");
    scanf("%d", &d); 
    dec_hex(d, hex);
    printf("%s\n", hex);
    return 0;
}
