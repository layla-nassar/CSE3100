#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 
#include "matrix.h"

#define     NUM_THREADS     sysconf(_SC_NPROCESSORS_ONLN) 

typedef struct {
    unsigned int id;
    unsigned int start_row;
    unsigned int end_row;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void * thread_main(void * p_arg)
{
    thread_arg_t *arg = (thread_arg_t *) p_arg;

    for (unsigned int i = arg->start_row; i < arg->end_row; i++) {
        for (unsigned int j = 0; j < arg->n->ncols; j++) {
            TElement sum = 0;
            for (unsigned int k = 0; k < arg->m->ncols; k++) {
                sum += arg->m->data[i][k] * arg->n->data[k][j];
            }
            arg->t->data[i][j] = sum;
        }
    }

    return NULL;
}

TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (m == NULL || n == NULL || m->ncols != n->nrows)
        return NULL;

    TMatrix *t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    unsigned int num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];

    unsigned int chunk_size = (m->nrows + num_threads - 1) / num_threads;

    for (unsigned int i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;

        args[i].start_row = i * chunk_size;
        args[i].end_row = (i + 1) * chunk_size > m->nrows ? m->nrows : (i + 1) * chunk_size;

        if (pthread_create(&threads[i], NULL, thread_main, &args[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (unsigned int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return t;
}
