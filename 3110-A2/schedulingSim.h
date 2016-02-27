/*************
schedulingSim.h -- Public interface for schedulingSim program in schedulingSim.c
Last updated:  3:20 PM February-26-16

Matt Sampson, 0888047
**************/

#ifndef SCHEDULINGSIM_H
#define SCHEDULINGSIM_H

#include <stdio.h>
#include <stdlib.h>


typedef struct BurstTimes{
	int CPU;
	int IO;
	int burstNum;
	void *next;
} BurstTimes;

typedef struct {
	int arrivalTime;
	BurstTimes *initBurst;
	int IO_timeRemaining;
	int finishTime;
	int threadx;
	int bursts;
} Thread;

typedef struct {
	int processNum;
	Thread threads[50];
	int numThreads;
	void *next;
} Process;

typedef struct {
	Thread *readyThread;
	void *next;
}Queue;

void getThreads(Process *process, int numThreads);
void createReadyQueue(Process *process, Queue *readyQueue, int clockTime);

#endif