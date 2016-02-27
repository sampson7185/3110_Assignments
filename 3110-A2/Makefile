CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g -c

all: schedulingSim

schedulingSim: schedulingSim.o
	$(CC) schedulingSim.o -o schedulingSim

schedulingSim.o: schedulingSim.c
	$(CC) $(CFLAGS) schedulingSim.c

clean:
	rm *o schedulingSim