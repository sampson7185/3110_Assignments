/*holes.h

This is the public interface for holes.c

Created: March 19th, 2016
Author: Matt Sampson
Contact: msamps01@mail.uoguelph.ca*/

typedef struct Process
{
	char processID;
	int processSize;
	int timesSwapped;
	int timeInMem;
	void *next;
}Process;

typedef struct queue
{
	Process *current;
	void *next;
}queue;

void simBF(Process *firstProcess);
void simWF(Process *firstProcess);
void simNF(Process *firstProcess);
void simFF(Process *firstProcess);
void createQueue(Process *firstProcess, queue *readyQueue);
void addTimeInMem(Process *firstProcess, char memory[]);
void swapProcess(Process *firstProcess, char memory[], queue *readyQueue);
void printMessage(Process *loadedProcess, char memory[], int last);