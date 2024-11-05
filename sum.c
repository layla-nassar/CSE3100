#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#define MAX 1000

int n;
unsigned long sum = 0;
pthread_mutex_t mutex;

void* calculate(void* threadarg)
{
    pthread_mutex_lock(&mutex); //fill in one line of code below

    if(n > 0)
    {
        sum += n*n;
        //do not delete the following line of code
        //this line helps us to see wrong results
        sleep(0);
        n--;
    }
    
    pthread_mutex_unlock(&mutex); //fill in one line of code below
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s n (1 - 500)\n", argv[0]);
        return -1;
    }

    n = atoi(argv[1]);
    assert(n >= 1 && n <= MAX/2);

    //save n to k for later use
    int k = n;
    int m = 2 * n;

    pthread_mutex_init(&mutex, NULL); //fill in one line of code below

    pthread_t threads[MAX];
    int rc, t;    
    for( t = 0; t < m; t++ ) {
        rc = pthread_create(&threads[t], NULL, calculate, NULL);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for( t = 0; t < m; t++ ) 
    {
        rc = pthread_join(threads[t], NULL);
        if(rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_mutex_destroy(&mutex); //fill in one line of code below

    unsigned long correct_sum = k * (k + 1) * (2 * k + 1) / 6;
    printf("thread sum : %ld correct sum : %ld\n", sum, correct_sum);
    return 0;
}
