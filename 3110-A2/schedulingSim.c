#include "schedulingSim.h"


int main(int argc, char const *argv[]) {
	Process *firstProcess;
	Thread *nextThread;
	Process *head;
	BurstTimes *temp;
	int numProcesses = 0, switchThread = 0, switchProcess = 0, clockTime = 0;
	int idleTime = 0, nextThreadParent = 0, currentProcess = 0, firstEvent = 1;
	int notFinished = 1;

	firstProcess = malloc(sizeof(Process));
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
		head = firstProcess;
		while (head->next != NULL)
			head = head->next;
		head->next = nextProcess;
	}
	//this loop runs until all threads in all processes are finished
	while (notFinished) {
		head = firstProcess;

		//find earliest arrival time that isnt doing IO and isnt finished
		//policy on same arrival time is to take one in earliest process
		while (head != NULL) {
			for (int j = 0; j < head->numThreads; j++) {
				if (head->threads[i].arrivalTime <= clockTime && head->threads[i].IO_timeRemaining <= 0) {
					if (head->threads[i].finishTime != 0) {
						if (nextThread == NULL) {
							nextThread = head->threads[i];
							nextThreadParent = head->processNum;
						}
						else if (nextThread.arrivalTime > head->threads[i].arrivalTime) {
							nextThread = head->threads[i];
							nextThreadParent = head->processNum;
						}
					}
				}
			}
			head = head->next;
		}
		//nextThread now points to the thread that is ready and has the lowest arrival time
		//check if thread is within process or in another one
		if (firstEvent) {
			currentProcess = nextThreadParent;
			firstEvent = 0;
		}
		else {
			if (currentProcess == nextThreadParent)
				clockTime += switchThread;
			else
				clockTime += switchProcess;
		}
		//add processing time to timeClock
		timeClock += nextThread.initBurst->CPU;
		nextThread.IO_timeRemaining = nextThread.initBurst->IO;
		//if the process is done, record finish time
		if (nextThread.initBurst->IO == 0)
			nextThread.finishTime = timeClock;
		//move initburst to next burst
		if (nextThread.initBurst->next != NULL) {
			temp = nextThread.initBurst->next;
			free(nextThread.initBurst);
			nextThread.initBurst = temp;
		}
		//check finished
		head = firstProcess;
		while (head != NULL) {
			notFinished = checkFinished(head);
			head = head->next;
		}
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
		finalBurst->IO = 0;
		BurstTimes *head;
		head = firstBurst;
		while (head->next != NULL)
			head = head->next;
		head->next = finalBurst;
	}
	return;
}

int checkFinished(Process *process) {
	for (int i = 0; i < process->numThreads; i++) {
		if (process->threads[i].finishTime)
	}
}
