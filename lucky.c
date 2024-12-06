#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#define MAX 100000
#define SIZE 10000

int n = SIZE;           
int v[SIZE][SIZE];      // 2D array
pthread_mutex_t mutex;  // a mutex
int r[MAX];             // 1D array to save results
int count = 0;          // number of items in the array r

typedef struct thread_arg_tag {
    int row1, row2;
} thread_arg_t;

void die(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    if (errno) perror("errno");
    exit(EXIT_FAILURE);
}

void check_pthread_return(int rv, char *msg) {
    if (rv == 0) return;
    errno = rv;
    die(msg);
}
//TODO:
//implement the following function to append i*n + j to
//the end of array r
//and update count
//note multiple thread could call this function simutaneously

void append(int i, int j) {
    pthread_mutex_lock(&mutex);
    r[count++] = i * n + j;
    pthread_mutex_unlock(&mutex);
}

int lucky_square(int i, int j) {
    int digits[10] = {0};

    if (v[i][j] != 7)
        return 0;

    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
            int num = v[i + k][j + l];
            if (num < 1 || num > 9 || digits[num])
                return 0;
            digits[num] = 1;
        }
    }
    return 1;
}

void* thread_main(void* thread_arg) {
    thread_arg_t* arg = thread_arg;
    int row1 = arg->row1;
    int row2 = arg->row2;

    for (int i = row1; i <= row2; i++) {
        for (int j = 1; j < n - 1; j++) {
            if (lucky_square(i, j)) append(i, j);
        }
    }
    pthread_exit(NULL);
}

int int_cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s seed n_threads\n", argv[0]);
        return 0;
    }

    int seed = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    srand(seed);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            v[i][j] = rand() % 10;
        }
    }

    pthread_mutex_init(&mutex, NULL);

    pthread_t thread_arr[n_threads];
    thread_arg_t thread_args[n_threads];
    int rows_per_thread = n / n_threads;

    for (int i = 0; i < n_threads; i++) {
        thread_args[i].row1 = i * rows_per_thread + 1;
        thread_args[i].row2 = (i == n_threads - 1) ? n - 2 : (i + 1) * rows_per_thread;
        pthread_create(&thread_arr[i], NULL, thread_main, &thread_args[i]);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(thread_arr[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Total lucky squares: %d\n", count);
    qsort(r, count, sizeof(int), int_cmp);
    for (int i = 0; i < (10 < count ? 10 : count); i++) {
        printf("(%d, %d)\n", r[i] / n, r[i] % n);
    }

    return 0;
}
