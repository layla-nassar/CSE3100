#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// write an integer to a pipe
void write_int(int pd, int value) { write(pd, &value, sizeof(int)); }
// read an integer from a pipe
// the function returns the number of bytes read
int read_int(int pd, int *value) { return read(pd, value, sizeof(int)); }

void dice_dist(int n) {
  int pd1[2];
  if (pipe(pd1) < 0) {
    perror("Error.");
    exit(-1);
  }

  int pd2[2];
  if (pipe(pd2) < 0) {
    perror("Error.");
    exit(-1);
  }

  pid_t pid = fork();
  if (pid == 0) {
    // Process A
    srand(3100);
    int v;
    close(pd2[1]);  // Close write end of pd2 in child A
    close(pd1[0]);  // Close read end of pd1 in child A
    while (read_int(pd2[0], &v) > 0) {
      int roll = (rand() % 6) + 1;
      write_int(pd1[1], roll);
    }
    close(pd2[0]);
    close(pd1[1]);
    exit(0);
  }

  int pd3[2];
  if (pipe(pd3) < 0) {
    perror("Error.");
    exit(-1);
  }

  int pd4[2];
  if (pipe(pd4) < 0) {
    perror("Error.");
    exit(-1);
  }

  pid_t pid1 = fork();
  if (pid1 == 0) {
    // Process B
    srand(3504);
    int v;
    close(pd4[1]);  // Close write end of pd4 in child B
    close(pd3[0]);  // Close read end of pd3 in child B
    while (read_int(pd4[0], &v) > 0) {
      int roll = (rand() % 6) + 1;
      write_int(pd3[1], roll);
    }
    close(pd4[0]);
    close(pd3[1]);
    exit(0);
  }

  close(pd2[0]);  // Close read end of pd2 in parent
  close(pd4[0]);  // Close read end of pd4 in parent
  close(pd1[1]);  // Close write end of pd1 in parent
  close(pd3[1]);  // Close write end of pd3 in parent

  int dist[11] = {0};
  int v1, v2;
  for (int i = 0; i < n; i++) {
    write_int(pd2[1], 1);
    write_int(pd4[1], 1);
    if (read_int(pd1[0], &v1) <= 0 || read_int(pd3[0], &v2) <= 0) break;
    int sum = v1 + v2;
    dist[sum - 2]++;
  }

  close(pd2[1]);  // Signal end of input to process A
  close(pd4[1]);  // Signal end of input to process B
  close(pd1[0]);
  close(pd3[0]);

  wait(NULL);  // Wait for child A to exit
  wait(NULL);  // Wait for child B to exit

  for (int i = 2; i <= 12; i++) {
    printf("%02d %d\n", i, dist[i - 2]);
  }
}
