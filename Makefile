CC=gcc
CFLAGS=-Wall -g -pthread -std=c99
TARGETS=lucky game

all: $(TARGETS)

lucky: lucky.c
	$(CC) $(CFLAGS) -o $@ $^

game: game.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o *~ $(TARGETS) a.out
