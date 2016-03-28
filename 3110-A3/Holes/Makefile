CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g -c

all: holes

holes: holes.o
	$(CC) holes.o -o holes

holes.o: holes.c holes.h
	$(CC) $(CFLAGS) holes.c

clean:
	rm *o holes