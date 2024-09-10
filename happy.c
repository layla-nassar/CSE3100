#include <stdio.h>   // For input/output functions like printf and scanf
#include <stdlib.h>  // For standard library functions (not specifically needed here)

int sum_of_squares(int num) {
    int sum = 0;  // Initialize sum to 0
    while (num > 0) {  // Loop to process each digit of the number
        int digit = num % 10;  // Extract the last digit of num
        sum += digit * digit;  // Add the square of the digit to the sum
        num /= 10;  // Remove the last digit from num (integer division)
    }
    return sum;  // Return the sum of squares of digits
}

int main() {
    int n;  // Declare variable n to hold the user's input number
    printf("n = ");  // Prompt the user to input a number
    scanf("%d", &n);  // Read the user's input and store it in n

    int current = n;  // Initialize current to the value of n, to perform the operations on

    // Loop until current becomes 1 (happy) or 4 (unhappy)
    while (current != 1 && current != 4) {
        current = sum_of_squares(current);  // Calculate the sum of the squares of digits
        printf("%d\n", current);  // Print the current value after squaring digits
    }

    // Check if the number is happy or not
    if (current == 1) {
        printf("%d is a happy number.\n", n);  // If current reached 1, n is a happy number
    } else {
        printf("%d is NOT a happy number.\n", n);  // If current reached 4, n is not a happy number
    }

    return 0;  // End the program successfully
}
