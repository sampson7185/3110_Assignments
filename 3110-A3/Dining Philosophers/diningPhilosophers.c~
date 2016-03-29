/*diningPhilosophers.c

This program simulates the Dining philosophers problem

Created: March 19th, 2016
Author: Matt Sampson
Contact: msamps01@mail.uoguelph.ca*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
	int numberPhil;
	int err = 0;
	int i = 0;

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
	pthread_t tid[numberPhil];
	//create number of semaphores = to number of philosophers
	sem_t chopstick[numberPhil];
	for (int i = 0; i < numberPhil; i++) {
		sem_init(&chopstick[i], 0, 1);
	}
	//create threads = to 1 less than number of semaphores
	//using algorithm where I'm letting 1 less than total number of chopsticks
	//to prevent deadlock
	while (i < numberPhil - 1) {
		//err = pthread_create(&(tid[i]), NULL, &simPhil, &chopstick);
		if (err != 0)
			printf("Can't create thread: [%s]\n", strerror(err));
		i++;
	}
	return 0;
}