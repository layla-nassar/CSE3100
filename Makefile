CC=gcc
CFLAGS=-Wall -g -std=c99
TARGETS=arithmetic dice

all: $(TARGETS)

arithmetic: arithmetic.c redirect.c redirect.h
	$(CC) $(CFLAGS) -o $@ $^

dice: run-dice.h run-dice.c dice.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o *~ $(TARGETS) a.out