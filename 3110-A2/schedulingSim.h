/*************
schedulingSim.h -- Public interface for schedulingSim program in schedulingSim.c
Last updated:  3:20 PM February-26-16

Matt Sampson, 0888047
**************/

#ifndef SCHEDULINGSIM_H
#define SCHEDULINGSIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	int parentProcess;
	int turnaroundTime;
	int serviceTime;
	int IOtime;
} Thread;

typedef struct {
	int processNum;
	Thread threads[50];
	int numThreads;
	void *next;
} Process;

typedef struct Queue{
	Thread *current;
	void *next;
}Queue;

void getThreads(Process *process, int numThreads, int parent);
int checkFinished(Process *process, int oldClockTime);
void simFCFS(Queue *readyQueue, int *firstEvent, int *currentProcess, int *clockTime, 
	int switchProcess, int switchThread, int *switchTime);
void simRR(Queue *readyQueue, int *firstEvent, int *currentProcess, int *clockTime, 
	int switchProcess, int switchThread, int timeQuantum, int *switchTime);
void printBasic(Process *firstProcess,int rrScheduling,int timeQuantum,int clockTime,int switchTime);
void printDetailed(Process *firstProcess);

#endif