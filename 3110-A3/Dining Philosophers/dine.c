/*dine.c

This program simulates the Dining philosophers problem

Created: March 19th, 2016
Author: Matt Sampson
Contact: msamps01@mail.uoguelph.ca*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int numberPhil;
int numberEat;

void *simPhil(void *arg) {
	int threadID = *((int*)arg);
	int i = 0;
	pthread_mutex_t lock[numberPhil];
	int leftChopstick = 0, rightChopstick = 0;

	leftChopstick = threadID - 1;
	rightChopstick = threadID;
	if (threadID == numberPhil + 1)
		rightChopstick = 0;
	//create number of mutexes = to number of philosophers
	for (i = 0; i < numberPhil; i++) {
		pthread_mutex_init(&lock[i], NULL);
	}
	i = 0;
	while (i < numberEat) {
		pthread_mutex_lock(&lock[leftChopstick]);
		pthread_mutex_lock(&lock[rightChopstick]);
		printf("Philosopher %d is eating.\n", threadID);
		sleep(1);
		i++;
		pthread_mutex_unlock(&lock[leftChopstick]);
		pthread_mutex_unlock(&lock[rightChopstick]);
		printf("Philosopher %d is thinking.\n", threadID);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	int err = 0;
	int i = 1;
	int test[200];

	if (argc < 3) {
		fprintf(stderr,"Not enough arguments, please provide the number of philosophers and "
			"how many times each needs to eat.\n");
		exit(-1);
	}
	else if (argc > 3) {
		fprintf(stderr,"Too many arguments, please provide the number of philosophers and "
			"how many times each needs to eat.\n");
		exit(-1);
	}
	numberPhil = atoi(argv[1]);
	numberEat = atoi(argv[2]);
	pthread_t tid[numberPhil];
	//create threads = to 1 less than number of semaphores
	//using algorithm where I'm letting 1 less than total number of chopsticks
	//to prevent deadlock
	while (i <= numberPhil - 1) {
		test[i - 1] = i;
		err = pthread_create(&(tid[i]), NULL, &simPhil, &test[i - 1]);
		if (err != 0)
			printf("Can't create thread: [%s]\n", strerror(err));
		i++;
	}
	i = 0;
	while(i < numberPhil - 1) {
		pthread_join(tid[i], NULL);
		i++;
	}
	i = 0;
	//this is the final philosopher eating
	while (i < numberEat) {
		printf("Philosopher %d is eating.\n", threadID);
		sleep(1);
		rintf("Philosopher %d is thinking.\n", threadID);
	}
	return 0;
}
