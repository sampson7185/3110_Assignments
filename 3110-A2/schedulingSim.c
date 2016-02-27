#include "schedulingSim.h"


int main(int argc, char const *argv[]) {
	Process *firstProcess;
	firstProcess = malloc(sizeof(Process));
	Process *head;
	Queue *readyQueue;
	readyQueue = malloc(sizeof(readyQueue));
	int numProcesses = 0, switchThread = 0, switchProcess = 0, clockTime = 0;
	int idleTime = 0;

	//get starting line of info
	fscanf(stdin,"%d %d %d", &numProcesses, &switchThread, &switchProcess);
	fscanf(stdin,"%d %d", &firstProcess->processNum, &firstProcess->numThreads);
	getThreads(firstProcess, firstProcess->numThreads);
	for (int i = 1; i < numProcesses; i++ ) {
		Process *nextProcess;
		nextProcess = malloc(sizeof(Process));
		fscanf(stdin,"%d %d", &nextProcess->processNum, &nextProcess->numThreads);
		getThreads(nextProcess, nextProcess->numThreads);
		//add process to linked list
		Process *head;
		head = firstProcess;
		while (head->next != NULL)
			head = head->next;
		head->next = nextProcess;
	}
	//create a queue of processes that are ready
	createReadyQueue(firstProcess,readyQueue);
	head = firstProcess->next;
	for (int i = 1; i < numProcesses; i++) {
		createReadyQueue(head,readyQueue,clockTime);
		head = head->next;
	}
	return 0;
}

void getThreads(Process *process, int numThreads) {
	for (int i = 0; i < numThreads; i++) {
		fscanf(stdin,"%d %d %d", &process->threads[i].threadx, &process->threads[i].arrivalTime, &process->threads[i].bursts);
		BurstTimes *firstBurst;
		firstBurst = malloc(sizeof(BurstTimes));
		if (process->threads[i].bursts == 1) {
			fscanf(stdin,"%d %d", &firstBurst->burstNum, &firstBurst->CPU);
			continue;
		}
		else
			fscanf(stdin,"%d %d %d", &firstBurst->burstNum, &firstBurst->CPU, &firstBurst->IO);
		process->threads[i].initBurst = firstBurst;
		for (int j = 1; j < process->threads[i].bursts - 1; j++) {
			BurstTimes *nextBurst;
			nextBurst = malloc(sizeof(BurstTimes));
			fscanf(stdin,"%d %d %d", &nextBurst->burstNum, &nextBurst->CPU, &nextBurst->IO);
			//attach to linked list
			BurstTimes *head;
			head = firstBurst;
			while (head->next != NULL)
				head = head->next;
			head->next = nextBurst;
		}
		//get final burst
		BurstTimes *finalBurst;
		finalBurst = malloc(sizeof(BurstTimes));
		fscanf(stdin,"%d %d", &finalBurst->burstNum, &finalBurst->CPU);
		BurstTimes *head;
		head = firstBurst;
		while (head->next != NULL)
			head = head->next;
		head->next = finalBurst;
	}
	return;
}

void createReadyQueue(Process *process, Queue *readyQueue, int clockTime) {
	Queue *head;
	for (int i = 0; i < process->numThreads; i++) {
		//check if thread has arrived yet and that it isnt finished
		if (process->threads[i].arrivalTime <= clockTime && process->threads[i].finishTime == 0) {
			//check if thread is doing IO
			if (process->threads[i].IO_timeRemaining <= 0) {
				//thread has arrived and is not doing IO, so its ready
				if (readyQueue->readyThread == NULL) {
					readyQueue->readyThread = process->threads[i];
				}
				else {
					//create new queue node
					Queue queueNode;
					queueNode = malloc(sizeof(Queue));
					queueNode->readyThread = process->threads[i];
					head = readyQueue->next;
					while(head->next != NULL)
						head = head->next;
					head->next = queueNode;
				}
			}
		}
	}
}