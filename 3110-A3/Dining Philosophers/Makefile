CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g -c

all: dine

dine: dine.o
	$(CC) -pthread dine.o -o dine

dine.o: dine.c
	$(CC) $(CFLAGS) dine.c

clean:
	rm *o dine