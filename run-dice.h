#ifndef RUN_DICE_H
#define RUN_DICE_H
//write an integer to a pipe
void write_int(int pd, int value);

//read an integer from a pipe
//the function returns the number of bytes read
int read_int(int pd, int *value);
void dice_dist(int n);
#endif // RUN_DICE_H