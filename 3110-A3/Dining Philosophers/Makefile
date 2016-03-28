CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g -c

all: diningPhilosophers

diningPhilosophers: diningPhilosophers.o
	$(CC) -pthread diningPhilosophers.o -o dine

diningPhilosophers.o: diningPhilosophers.c
	$(CC) $(CFLAGS) diningPhilosophers.c

clean:
	rm *o dine