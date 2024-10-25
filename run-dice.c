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
  int pd1[2], pd2[2], pd3[2], pd4[2];

  //pipe creation
  if (pipe(pd1) < 0 || pipe(pd2) < 0 || pipe(pd3) < 0 || pipe(pd4) < 0) {
    perror("Error creating pipes.");
    exit(-1);
  }

  pid_t pid = fork();
  if (pid == 0) {
  
    srand(3100);
    close(pd2[1]);  
    close(pd1[0]);  
    close(pd3[0]); close(pd3[1]); 
    close(pd4[0]); close(pd4[1]);

    int v;
    while (read_int(pd2[0], &v) > 0) {
      int roll = (rand() % 6) + 1;
      write_int(pd1[1], roll);
    }
    
    close(pd2[0]);
    close(pd1[1]);
    exit(0);
  }

  pid_t pid1 = fork();
  if (pid1 == 0) {

    srand(3504);
    close(pd4[1]);  
    close(pd3[0]);  
    close(pd1[0]); close(pd1[1]);
    close(pd2[0]); close(pd2[1]);

    int v;
    while (read_int(pd4[0], &v) > 0) {
      int roll = (rand() % 6) + 1;
      write_int(pd3[1], roll);
    }
    
    close(pd4[0]);
    close(pd3[1]);
    exit(0);
  }

  close(pd2[0]);  
  close(pd4[0]);  
  close(pd1[1]);  
  close(pd3[1]);  

  int dist[11] = {0};
  int v1, v2;
  for (int i = 0; i < n; i++) {
    write_int(pd2[1], 1);
    write_int(pd4[1], 1);
    if (read_int(pd1[0], &v1) <= 0 || read_int(pd3[0], &v2) <= 0) break;
    int sum = v1 + v2;
    dist[sum - 2]++;
  }
  
  close(pd2[1]);
  close(pd4[1]);
  close(pd1[0]);
  close(pd3[0]);

  wait(NULL);
  wait(NULL);

  for (int i = 2; i <= 12; i++) {
    printf("%02d %d\n", i, dist[i - 2]);
  }
}
