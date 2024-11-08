#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "linked-list.h"

#define MAX 10

typedef struct {
    node *head, *tail;
    pthread_mutex_t mutex;
} list_t;

typedef struct {
    int size;
    int buf[MAX][3];
    int remain;
    int counts[3];            // current indexes
    pthread_mutex_t mutex;
    pthread_cond_t produce_cond;
    pthread_cond_t consume_cond;
} two_d_buffer;

void add_to_buffer(int item, int col, two_d_buffer *p) {
    pthread_mutex_lock(&p->mutex);

    while (p->counts[col] >= p->size) {
        pthread_cond_wait(&p->produce_cond, &p->mutex);
    }

    p->buf[p->counts[col]][col] = item;
    p->counts[col]++;
    pthread_cond_signal(&p->consume_cond);
    pthread_mutex_unlock(&p->mutex);
}

void remove_from_buffer(int *a, int *b, int *c, two_d_buffer *p) {
    pthread_mutex_lock(&p->mutex);

    while (p->counts[0] == 0 || p->counts[1] == 0 || p->counts[2] == 0) {
        pthread_cond_wait(&p->consume_cond, &p->mutex);
    }

    *a = p->buf[0][0];
    *b = p->buf[0][1];
    *c = p->buf[0][2];

    for (int i = 1; i < p->counts[0]; i++) {
        p->buf[i - 1][0] = p->buf[i][0];
        p->buf[i - 1][1] = p->buf[i][1];
        p->buf[i - 1][2] = p->buf[i][2];
    }

    p->counts[0]--;
    p->counts[1]--;
    p->counts[2]--;

    pthread_cond_signal(&p->produce_cond);
    pthread_mutex_unlock(&p->mutex);
}

void prepare(int item) {
    usleep((item + 1) * 100);
}

void free_all_nodes(list_t *p) {
    node *current = p->head;
    while (current != NULL) {
        node *next = current->next;
        free(current);
        current = next;
    }
}

struct thread_data {
    int id;
    list_t *p;
    two_d_buffer *q;
    int total;                // total items produced by a producer
    pthread_barrier_t *p_barrier;
};

void* thread_consume(void* threadarg) {
    struct thread_data* my_data = (struct thread_data*) threadarg;
    list_t *p = my_data->p;

    node *n1 = create_node(0);
    node *n2 = create_node(1);
    node *n3 = create_node(2);

    add_last(&p->head, &p->tail, n1);
    add_last(&p->head, &p->tail, n2);
    add_last(&p->head, &p->tail, n3);

    pthread_barrier_wait(my_data->p_barrier);

    int a, b, c;
    remove_from_buffer(&a, &b, &c, my_data->q);
    printf("consumer %04d (%d %d %d)\n", my_data->id, a, b, c);
    pthread_exit(NULL);
}

void* thread_produce(void* threadarg) {
    struct thread_data* my_data = (struct thread_data*) threadarg;
    pthread_barrier_wait(my_data->p_barrier);

    two_d_buffer *q = my_data->q;
    while (1) {
        pthread_mutex_lock(&q->mutex);
        if (q->remain <= 0) {
            pthread_mutex_unlock(&q->mutex);
            break;
        }
        q->remain -= 3; 
        pthread_mutex_unlock(&q->mutex);

        for (int i = 0; i < 3; i++) {
            prepare(i);
            add_to_buffer(i, i, q);
            my_data->total++;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s n_consumer n_producer buffer_size\n", argv[0]);
        return -1;
    }
    int n_consumer = atoi(argv[1]);
    assert(n_consumer <= 3000);
    int n_producer = atoi(argv[2]);
    assert(n_producer <= 3000);
    int size = atoi(argv[3]);
    assert(size <= MAX);

    // Initialize the list
    list_t *p = (list_t *)malloc(sizeof(list_t));
    if (p == NULL) {
        perror("Cannot allocate memory.\n");
        return -1;
    }
    p->head = NULL;
    p->tail = NULL;
    pthread_mutex_init(&p->mutex, NULL);

    // Initialize the 2D buffer
    two_d_buffer *q = malloc(sizeof(two_d_buffer));
    q->size = size;
    q->remain = 3 * n_consumer;
    q->counts[0] = 0; q->counts[1] = 0; q->counts[2] = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->produce_cond, NULL);
    pthread_cond_init(&q->consume_cond, NULL);

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, n_consumer + n_producer); 
    pthread_t threads[n_consumer + n_producer];
    struct thread_data thread_data_array[n_consumer + n_producer];
    int rc, t;

    for (t = 0; t < n_consumer; t++) {
        thread_data_array[t].id = t;
        thread_data_array[t].p = p;
        thread_data_array[t].q = q;
        thread_data_array[t].total = 0;
        thread_data_array[t].p_barrier = &barrier;

        rc = pthread_create(&threads[t], NULL, thread_consume, &thread_data_array[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < n_producer; t++) {
        thread_data_array[n_consumer + t].id = t;
        thread_data_array[n_consumer + t].p = p;
        thread_data_array[n_consumer + t].q = q;
        thread_data_array[n_consumer + t].total = 0;
        thread_data_array[n_consumer + t].p_barrier = &barrier;

        rc = pthread_create(&threads[n_consumer + t], NULL, thread_produce, &thread_data_array[n_consumer + t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < n_consumer + n_producer; t++) {
        rc = pthread_join(threads[t], NULL);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    int total = 0;
    for (t = 0; t < n_producer; t++) {
        total += thread_data_array[n_consumer + t].total;
    }

    printf("total = %d\n", total);

    // Free all nodes in the linked list
    free_all_nodes(p);
    pthread_mutex_destroy(&p->mutex);
    free(p);

    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->consume_cond);
    pthread_cond_destroy(&q->produce_cond);
    free(q);

    pthread_barrier_destroy(&barrier);
    return 0;
}
