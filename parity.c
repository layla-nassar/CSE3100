#include <stdio.h>

int main(void) {
    unsigned int v = 19;  // example value
    char parity = 0;
    
    // Calculate the parity
    while (v) {
        parity = !parity;
        v = v & (v - 1);  // clear the lowest set bit
    }
    
    return 0;
}