#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define MAX 80
#define SET_BIT(v, i) (v) |= (1 << (i))
#define CHECK_BIT(v, i) (v) & (1 << (i))

typedef struct {
  char data;                     // player's guess (a letter)
  char *display;                 // current game status display
  int ready;                     // new letter ready
  int checked;                   // used by SET_BIT and CHECK_BIT
  int flag;                      // flag to indicate all checked
  pthread_mutex_t mutex;         // mutex
  pthread_cond_t ready_cond;     // condition variable that assistants wait on
  pthread_cond_t processed_cond; // condition variable that the player waits on
} data_t;

typedef struct {
  int id;        // id
  char letter;   // letter assigned
  data_t *pdata; // pointer to data_t
} thread_arg_t;

void *assistant(void *t) {
  thread_arg_t *arg = t;
  int my_id = arg->id;
  char letter = arg->letter;
  data_t *pdata = arg->pdata;

  while (1) {
    pthread_mutex_lock(&pdata->mutex);


    while (!pdata->ready && pdata->data >= 0)
      pthread_cond_wait(&pdata->ready_cond, &pdata->mutex);

    
    if (pdata->data < 0) {
      pthread_mutex_unlock(&pdata->mutex);
      break;
    }

    
    if (pdata->data == letter) {
      pdata->display[my_id] = letter;
    }

    
    SET_BIT(pdata->checked, my_id);

    
    if (pdata->checked == pdata->flag) {
      pdata->ready = 0;
      pthread_cond_signal(&pdata->processed_cond);
    }

    pthread_mutex_unlock(&pdata->mutex);
  }
  pthread_exit(NULL);
}

void *player(void *t) {
  thread_arg_t *arg = t;
  data_t *pdata = arg->pdata;
  int n = strlen(pdata->display);
  char v = 'a';

  while (1) {
    pthread_mutex_lock(&pdata->mutex);

    
    pdata->data = v;

    
    printf("%s\n", pdata->display);
    printf("%c\n", v);

    
    pdata->ready = 1;
    pdata->checked = 0;

    
    pthread_cond_broadcast(&pdata->ready_cond);

    
    while (pdata->ready)
      pthread_cond_wait(&pdata->processed_cond, &pdata->mutex);

    pthread_mutex_unlock(&pdata->mutex);

    
    int count = 0;
    for (int i = 0; i < n; i++) {
      if (pdata->display[i] != '-')
        count++;
    }
    if (count == n) {
      pdata->data = -1; 
      pthread_cond_broadcast(&pdata->ready_cond);
      printf("%s\n", pdata->display); 
      break;
    }

    v++; 
    if (v > 'z') {
      pdata->data = -1; 
      pthread_cond_broadcast(&pdata->ready_cond);
      break;
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int i, rv;
  data_t data;

  assert(argc == 2);
  int n = strlen(argv[1]);
  assert(n < MAX);
  char *word = argv[1];
  for (i = 0; i < n; i++)
    assert(word[i] >= 'a' && word[i] <= 'z');
  char display[MAX] = {0};

  int flag = 0;
  for (i = 0; i < n; i++)
    SET_BIT(flag, i);
  for (i = 0; i < n; i++)
    display[i] = '-';

  data.ready = 0; //no data is ready yet
  data.display = display;
  data.flag = flag;
  pthread_mutex_init(&data.mutex, NULL);
  pthread_cond_init(&data.ready_cond, NULL);
  pthread_cond_init(&data.processed_cond, NULL);

  pthread_t threads[n + 1];
  thread_arg_t thread_args[n + 1];

  // Initialize mutex and condition variable objects
  // Create assistant threads
  for (i = 0; i < n; i++) {
    thread_args[i].id = i;
    thread_args[i].letter = word[i];
    thread_args[i].pdata = &data;
    rv = pthread_create(&threads[i], NULL, assistant, &thread_args[i]);
    assert(rv == 0);
  }

  // Create the player thread
  thread_args[n].id = n;
  thread_args[n].letter = 0;
  thread_args[n].pdata = &data;
  rv = pthread_create(&threads[n], NULL, player, &thread_args[n]);
  assert(rv == 0);

  // Wait for all threads to complete
  for (i = 0; i < n + 1; i++) {
    pthread_join(threads[i], NULL);
  }

  // Clean up and exit 
  pthread_mutex_destroy(&data.mutex);
  pthread_cond_destroy(&data.ready_cond);
  pthread_cond_destroy(&data.processed_cond);

  return 0;
}