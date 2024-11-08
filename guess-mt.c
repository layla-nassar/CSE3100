#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

void die(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
}

void check_pthread_return(int rv, char *msg) {
    if (rv == 0)
        return;
    errno = rv;
    die(msg);
}

#define MAX_VALUE       1000
#define MSG_BUF_SIZE    100

typedef struct {
    int v;
    int num_attempts;
    char message[MSG_BUF_SIZE];
} gmn_t;

void gmn_init(gmn_t *pg, int seed) {
    srand(seed);
    pg->v = rand() % MAX_VALUE + 1;
    pg->num_attempts = 0;
    strcpy(pg->message, "You have not got the right number yet.\n");
}

int gmn_check(gmn_t *pg, int guess) {
    pg->num_attempts++;
    if (pg->v == guess) {
        snprintf(pg->message, MSG_BUF_SIZE, 
                 "It took you %d attempt(s) to guess the number %d.\n", 
                 pg->num_attempts, pg->v);
        return 0;
    }
    return (pg->v > guess) ? 1 : -1;
}

char *gmn_get_message(gmn_t *pg) {
    return pg->message;
}

int gmn_get_max() {
    return MAX_VALUE;
}

typedef struct thread_arg_tag {
    int seed;
    int max;
    int guess;
    int result;
    char message[MSG_BUF_SIZE];
    int status;
    pthread_mutex_t mutex;
    pthread_cond_t cond_guess;
    pthread_cond_t cond_result;
} thread_arg_t;

void my_msleep(int r) {
    struct timespec req = {0, (r % 100 + 1) * 1000000};
    nanosleep(&req, NULL);
}

enum { S_INIT, S_GUESS, S_RESULT, S_MAX };

void *thread_c(void *arg_in) {
    thread_arg_t *arg = arg_in;
    gmn_t gmn;
    gmn_init(&gmn, arg->seed);
    my_msleep(rand());

    pthread_mutex_lock(&arg->mutex);

    arg->max = gmn_get_max();
    arg->status = S_MAX;
    pthread_cond_signal(&arg->cond_guess);

    while (1) {
        while (arg->status != S_GUESS) {
            pthread_cond_wait(&arg->cond_guess, &arg->mutex);
        }
        int result = gmn_check(&gmn, arg->guess);
        arg->result = result;

        if (result == 0) {
            strcpy(arg->message, gmn_get_message(&gmn));
            arg->status = S_RESULT;
            pthread_cond_signal(&arg->cond_result);
            break;
        } else {
            arg->status = S_RESULT;
            pthread_cond_signal(&arg->cond_result);
        }
    }

    pthread_mutex_unlock(&arg->mutex);
    return NULL;
}

void *thread_p(void *arg_in) {
    thread_arg_t *arg = arg_in;
    int min = 1;
    int max;
    int guess;
    int result;

    my_msleep(arg->seed);

    pthread_mutex_lock(&arg->mutex);

    while (arg->status != S_MAX) {
        pthread_cond_wait(&arg->cond_guess, &arg->mutex);
    }
    max = arg->max;

    do {
        guess = (min + max) / 2;
        printf("My guess: %d\n", guess);
        arg->guess = guess;
        arg->status = S_GUESS;
        pthread_cond_signal(&arg->cond_guess);

        while (arg->status != S_RESULT) {
            pthread_cond_wait(&arg->cond_result, &arg->mutex);
        }

        result = arg->result;
        if (result > 0)
            min = guess + 1;
        else if (result < 0)
            max = guess - 1;
    } while (result != 0);

    printf("%s", arg->message);

    pthread_mutex_unlock(&arg->mutex);
    return NULL;
}

int main(int argc, char *argv[]) {
    int seed = 5050;

    for (int i = 1; i < argc; i++) {
        if (isdigit(argv[i][0])) {
            seed = atoi(argv[i]);
            if (seed <= 0)
                die("seed is 0 or it is too large.");
        } else {
            fprintf(stderr, "Usage: %s [<seed>]\n", argv[0]);
            return 1;
        }
    }

    thread_arg_t arg;
    arg.seed = seed;

    pthread_mutex_init(&arg.mutex, NULL);
    pthread_cond_init(&arg.cond_guess, NULL);
    pthread_cond_init(&arg.cond_result, NULL);

    pthread_t threadC, threadP;
    check_pthread_return(pthread_create(&threadC, NULL, thread_c, &arg), "Failed to create thread C");
    check_pthread_return(pthread_create(&threadP, NULL, thread_p, &arg), "Failed to create thread P");

    pthread_join(threadC, NULL);
    pthread_join(threadP, NULL);

    pthread_mutex_destroy(&arg.mutex);
    pthread_cond_destroy(&arg.cond_guess);
    pthread_cond_destroy(&arg.cond_result);

    return 0;
}
