#include <stdio.h>

int main(void) {    
    printf("Hello, World!\n");

int sum = 0;
int i = 2; 

while (i < 200) {
    sum += i;
    i += 2;
}

printf("%d\n", sum);

return 0;

}