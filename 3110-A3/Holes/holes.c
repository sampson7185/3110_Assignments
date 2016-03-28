/*holes.c

This program simulates the best fit, worst fit, next fit, and first fit algorithms
for filling holes in memory.

Created: March 19th, 2016
Author: Matt Sampson
Contact: msamps01@mail.uoguelph.ca*/
#include "holes.h"
#include <stdio.h>
#include <stdlib.h>

//this function resets the variables that are changing in each sim so that 
//the next sim can start with a clean slate
void resetProcesses(Process *firstProcess) {
	Process *head;

	head = firstProcess;
	while (head != NULL) {
		head->timeInMem = 0;
		head->timesSwapped = 0;
		head = head->next;
	}
}

int main(int argc, char const *argv[])
{
	FILE *fp;
	int EOFcheck = 0;
	Process *firstProcess;
	Process *nextProcess;
	Process *head;

	//check if user included a file to read from
	if (argc < 2) {
		fprintf(stderr,"Not enough arguments, please provide a file to read from.\n");
		exit(-1);
	}
	else if (argc > 2) {
		fprintf(stderr,"Too many arguments, please provide a file to read from.\n");
		exit(-1);
	}
	//open file
	fp = fopen(argv[1],"r");
	//read in first process
	firstProcess = malloc(sizeof(Process));
	EOFcheck = fscanf(fp, " %c %d", &firstProcess->processID, &firstProcess->processSize);
	firstProcess->timesSwapped = 0;
	//read in all subsequent processes
	while (EOFcheck != -1) {
        nextProcess = malloc(sizeof(Process));
        EOFcheck = fscanf(fp, " %c %d", &nextProcess->processID, &nextProcess->processSize);
        if (EOFcheck == -1)
        	break;
        nextProcess->timesSwapped = 0;
        head = firstProcess;
        while (head->next != NULL)
            head = head->next;
        head->next = nextProcess;
	}
	//pass list of process to each allocation algorithm
	simBF(firstProcess);
	resetProcesses(firstProcess);
	simWF(firstProcess);
	resetProcesses(firstProcess);
	simNF(firstProcess);
	resetProcesses(firstProcess);
	simFF(firstProcess);
	resetProcesses(firstProcess);
	fclose(fp);
	return 0;
}

//this function sims the best fit algorithm of memory allocation
void simBF(Process *firstProcess) {
	//this is a bitmap to simulate memory usage
	//it uses chars to keep track of which process is in that part of memory
	char memory[128];
	queue *readyQueue;
	int notFinished = 1;
	int freeCount = 0;
	int bestFit = 0;
	int willFit = 0;
	int lowestFreeCount = 128;
	queue *Qhead;

	//fill char array with spaces to signify that its empty 
	for (int i = 0; i < 128; i++) {
		memory[i] = ' ';
	}
	//fill queue
	readyQueue = malloc(sizeof(queue));
	createQueue(firstProcess,readyQueue);
	while (notFinished) {
		willFit = 0;
		//check if queue is empty
		if (readyQueue == NULL) {
			notFinished = 0;
			break;
		}
		//check if there is space in the array and find best fit
		for (int i = 0; i < 128; i++) {
			if (memory[i] == ' ')
				freeCount++;
			else {
				if (willFit && freeCount < lowestFreeCount) {
					lowestFreeCount = freeCount;
					bestFit = i - freeCount + 1;
				}
				freeCount = 0;
			}
			if (freeCount == readyQueue->current->processSize) {
				willFit = 1;
			}
			else if (freeCount < readyQueue->current->processSize) {
				willFit = 0;
			}
		}
		//if it will fit put it into the best fitting spot
		if (willFit) {
			for (int i = bestFit; i < bestFit + readyQueue->current->processSize; i++) {
				memory[i] = readyQueue->current->processID;
			}
			freeCount = 0;
			//print info of loaded process
      		printMessage(readyQueue->current, memory, 0);
			//move to the next process in the queue
			if (readyQueue != NULL) {
        		Qhead = readyQueue->next;
        		free(readyQueue);
        		readyQueue = NULL;
        		readyQueue = Qhead;
  			}
  			if (readyQueue == NULL) {
				notFinished = 0;
				break;
			}
  			//add to int to represent time spent in memory
  			addTimeInMem(firstProcess, memory);
		}
		else {
			//if not able to be put in memory, swap out the process that has
			//been in memory the longest
			swapProcess(firstProcess, memory, readyQueue);
		}
	}
	//print final message
	printMessage(firstProcess, memory, 1);	
}

//this function sims the worst fit algorithm of memory allocation
void simWF(Process *firstProcess) {
	//this is a bitmap to simulate memory usage
	//it uses chars to keep track of which process is in that part of memory
	char memory[128];
	queue *readyQueue;
	int notFinished = 1;
	int freeCount = 0;
	int worstFit = 0;
	int willFit = 0;
	int highestFreeCount = 0;
	queue *Qhead;

	//fill char array with spaces to signify that its empty 
	for (int i = 0; i < 128; i++) {
		memory[i] = ' ';
	}
	//fill queue
	readyQueue = malloc(sizeof(queue));
	createQueue(firstProcess,readyQueue);
	while (notFinished) {
		willFit = 0;
		//check if queue is empty
		if (readyQueue == NULL) {
			notFinished = 0;
			break;
		}
		//check if there is space in the array and find best fit
		for (int i = 0; i < 128; i++) {
			if (memory[i] == ' ')
				freeCount++;
			else {
				if (willFit && freeCount > highestFreeCount) {
					highestFreeCount = freeCount;
					worstFit = i - freeCount + 1;
				}
				freeCount = 0;
			}
			if (freeCount == readyQueue->current->processSize) {
				willFit = 1;
			}
			else if (freeCount < readyQueue->current->processSize) {
				willFit = 0;
			}
		}
		//if it will fit put it into the worst fitting spot
		if (willFit) {
			for (int i = worstFit; i < worstFit + readyQueue->current->processSize; i++) {
				memory[i] = readyQueue->current->processID;
			}
			freeCount = 0;
			//print info of loaded process
      		printMessage(readyQueue->current, memory, 0);
			//move to the next process in the queue
			if (readyQueue != NULL) {
        		Qhead = readyQueue->next;
        		free(readyQueue);
        		readyQueue = NULL;
        		readyQueue = Qhead;
  			}
  			if (readyQueue == NULL) {
				notFinished = 0;
				break;
			}
  			//add to int to represent time spent in memory
  			addTimeInMem(firstProcess, memory);
		}
		else {
			//if not able to be put in memory, swap out the process that has
			//been in memory the longest
			swapProcess(firstProcess, memory, readyQueue);
		}
	}
	//print final message
	printMessage(firstProcess, memory, 1);
}

//this function sims the next fit algorithm of memory allocation
void simNF(Process *firstProcess) {
	//this is a bitmap to simulate memory usage
	//it uses chars to keep track of which process is in that part of memory
	char memory[128];
	queue *readyQueue;
	int notFinished = 1;
	int freeCount = 0;
	queue *Qhead;
	int lastPlacement = 0;
	
	//fill char array with spaces to signify that its empty 
	for (int i = 0; i < 128; i++) {
		memory[i] = ' ';
	}
	//fill queue
	readyQueue = malloc(sizeof(queue));
	createQueue(firstProcess,readyQueue);
	//start while loop to sim algorithm unitl queue is empty
	while (notFinished) {
		freeCount = 0;
		//check if queue is empty
		if (readyQueue == NULL) {
			notFinished = 0;
			break;
		}
		//check if there is space in the array
		for (int i = lastPlacement; i < 128; i++) {
			if (memory[i] == ' ')
				freeCount++;
			else
				freeCount = 0;
			if (freeCount == readyQueue->current->processSize) {
				//large enough for current process in queue
				//go back to where free space starts and put process in memory
				i = i - freeCount + 1;
				for (int j = i; j < i + freeCount; j++)
					memory[j] = readyQueue->current->processID;
				//reset i to original position
				i = i + freeCount - 1;
				lastPlacement = i;
				freeCount = 0;
				//print info of loaded process
      			printMessage(readyQueue->current, memory, 0);
				//move to the next process in the queue
				if (readyQueue != NULL) {
            		Qhead = readyQueue->next;
            		free(readyQueue);
            		readyQueue = NULL;
            		readyQueue = Qhead;
      			}
      			if (readyQueue == NULL) {
					notFinished = 0;
					break;
				}
      			//add to int to represent time spent in memory
      			addTimeInMem(firstProcess, memory);
      		}
		}
		//if not able to be put in memory, swap out the process that has
		//been in memory the longest
		swapProcess(firstProcess, memory, readyQueue);
		lastPlacement = 0;
	}
	//print final message
	printMessage(firstProcess, memory, 1);
}

//this function sims the first fit algorithm of memory allocation
void simFF(Process *firstProcess) {
	//this is a bitmap to simulate memory usage
	//it uses chars to keep track of which process is in that part of memory
	char memory[128];
	queue *readyQueue;
	int notFinished = 1;
	int freeCount = 0;
	queue *Qhead;
	
	//fill char array with spaces to signify that its empty 
	for (int i = 0; i < 128; i++) {
		memory[i] = ' ';
	}
	//fill queue
	readyQueue = malloc(sizeof(queue));
	createQueue(firstProcess,readyQueue);
	//start while loop to sim algorithm unitl queue is empty
	while (notFinished) {
		freeCount = 0;
		//check if queue is empty
		if (readyQueue == NULL) {
			notFinished = 0;
			break;
		}
		//check if there is space in the array
		for (int i = 0; i < 128; i++) {
			if (memory[i] == ' ')
				freeCount++;
			else
				freeCount = 0;
			if (freeCount == readyQueue->current->processSize) {
				//large enough for current process in queue
				//go back to where free space starts and put process in memory
				i = i - freeCount + 1;
				for (int j = i; j < i + freeCount; j++)
					memory[j] = readyQueue->current->processID;
				//reset i to original position
				i = i + freeCount - 1;
				freeCount = 0;
				//print info of loaded process
      			printMessage(readyQueue->current, memory, 0);
				//move to the next process in the queue
				if (readyQueue != NULL) {
            		Qhead = readyQueue->next;
            		free(readyQueue);
            		readyQueue = NULL;
            		readyQueue = Qhead;
      			}
      			if (readyQueue == NULL) {
					notFinished = 0;
					break;
				}
      			//add to int to represent time spent in memory
      			addTimeInMem(firstProcess, memory);
      		}
		}
		//if not able to be put in memory, swap out the process that has
		//been in memory the longest
		swapProcess(firstProcess, memory, readyQueue);
	}
	//print final message
	printMessage(firstProcess, memory, 1);
}

//this function creates the ready queue at the beginning of each algorithm sim
void createQueue(Process *firstProcess, queue *readyQueue){
	queue *Qhead;
	queue *Qnext;
	Process *head;

	head = firstProcess;
    readyQueue->current = head;
    readyQueue->next = NULL;
    head = head->next;
	while(head != NULL) {
		Qhead = readyQueue;
		while (Qhead->next != NULL){
			Qhead = Qhead->next;
		}
		Qnext = malloc(sizeof(queue));
		Qnext->next = NULL;
        Qnext->current = head;
        Qhead->next = Qnext;
        head = head->next;
	}
}

//this function goes through the array looking for the search char
//and if found adds to an int to signify time spent in memory
void addTimeInMem(Process *firstProcess, char memory[]) {
	Process *head;

	head = firstProcess;
	while (head != NULL) {
		for (int i = 0; i < 128; i++) {
			if (memory[i] == head->processID){
				head->timeInMem++;
				break;
			}
		}
		head = head->next;
	}
}

//this function goes through the processes and swaps out the process that has
//been in memory the longest
void swapProcess(Process *firstProcess, char memory[], queue *readyQueue) {
	Process *head;
	queue *Qhead;
	queue *Qnext;
	int longest;
	char longestID;

	//find process with longest time in memory
	head = firstProcess;
	longest = head->timeInMem;
	longestID = head->processID;
	while (head != NULL) {
		if (longest < head->timeInMem) {
			longest = head->timeInMem;
			longestID = head->processID;
		}
		head = head->next;
	}
	//go through memory array and replace processID with blanks
	for (int i = 0; i < 128; i++) {
		if (memory[i] == longestID) {
			memory[i] = ' ';
		}
	}
	//add process to end of queue
	head = firstProcess;
	while (head != NULL) {
		if (longestID == head->processID && head->timesSwapped < 3) {
			head->timesSwapped++;
			if (head->timesSwapped == 3) {
				head->timeInMem = 0;
				break;
			}
			//add to end of queue
			Qhead = readyQueue;
			while (Qhead->next != NULL){
				Qhead = Qhead->next;
			}	
			Qnext = malloc(sizeof(queue));
			Qnext->next = NULL;
        	Qnext->current = head;
        	Qhead->next = Qnext;
        	head->timeInMem = 0;
		}
		head = head->next;
	}
}

//this function prints the message when each process is loaded and prints the
//final message when the sim is complete
void printMessage(Process *loadedProcess, char memory[], int last) {
	char uniqueProc[26];
	int uniqueCount = 0;
	int unique = 1;
	int emptySpaces = 0;
	int holeCount = 0;
	float memusage = 0;
	static float cumMemusage = 0;
	static int numCumMemusage = 0;
	static int totalLoads = 0;
	static int totalProcess = 0;
	static int totalHoles = 0;
	float currentCumMemusage = 0;
	float averProc = 0;
	float averHoles = 0;

	if (!last) {
		totalLoads++;
		for (int i = 0; i < 26; i++)
			uniqueProc[i] = ' ';

		printf("%c loaded, ", loadedProcess->processID);
		//determine number of processes in memory, holes, and % memory usage
		for (int i = 0; i < 128; i++) {
			//check current letter against unique processes to ensure proper
			//counting
			for (int j = 0; j < 26; j++) {
				if (uniqueProc[j] == memory[i]) {
					unique = 0;
					break;
				}
			}
			if (unique) {
				uniqueProc[uniqueCount] = memory[i];
				uniqueCount++;
			}
			if (memory[i] == ' ') {
				emptySpaces++;
			}
			unique = 1;
		}
		for (int i = 0; i < 128; i++) {
			//look for holes in the memory
			if (memory[i] == ' ') {
				for (int j = i; j < 128; j++) {
					if (memory[j] != ' ') {
						//found a hole
						holeCount++;
						i = j;
						break;
					}
				}
			}
		}
		totalProcess += uniqueCount;
		totalHoles += holeCount;
		memusage = 100.0 - (((float)emptySpaces / 128.0)*100.0);
		//add mem usage of current to cumulative and then divide by total 
		//number of times it was added to
		cumMemusage += memusage;
		numCumMemusage++;
		currentCumMemusage = cumMemusage / numCumMemusage;
		printf("#processes = %d, ", uniqueCount);
		printf("#holes = %d, ", holeCount);
		printf("%%memusage = %.0f, ", memusage);
		printf("cumulative %%mem = %.0f\n", currentCumMemusage);
	}
	else {
		printf("Total loads = %d, ", totalLoads);
		averProc = (float)totalProcess / (float)totalLoads;
		printf("average #processes = %.1f, ", averProc);
		averHoles = (float)totalHoles / (float)totalLoads;
		printf("average #holes = %.1f, ", averHoles);
		currentCumMemusage = cumMemusage / numCumMemusage;
		printf("cumulative %%mem = %.0f\n", currentCumMemusage);
		//reset all static variables for the next allocation algorithm
		totalHoles = 0;
		totalProcess = 0;
		totalLoads = 0;
		numCumMemusage = 0;
		cumMemusage = 0;
	}
}