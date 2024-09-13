# This is a comment line

#define variables so it is easier to make changes
CC=gcc
CFLAGS=-g -Wall -std=c99
TARGETS=ex-factorial

all: $(TARGETS)

ex-factorial: ex-factorial.c
	$(CC) $(CFLAGS) -o ex-factorial ex-factorial.c

clean:
	rm -rf *.o *~ $(TARGETS) a.out


