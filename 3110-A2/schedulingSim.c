#include "schedulingSim.h"

int main(int argc, char const *argv[]) {
    Process *firstProcess;
    Process *head;
    Queue *readyQueue;
    Queue *Qnext;
    Queue *Qhead;
    int numProcesses = 0, switchThread = 0, switchProcess = 0, clockTime = 0;
    int currentProcess = 0, firstEvent = 1, i = 0, dFlag = 0, vFlag = 0;
    int notFinished = 1, rrScheduling = 0, timeQuantum = 0;

    //find out which flags were entered
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i],"-d") == 0)
            dFlag = 1;
        else if (strcmp(argv[i],"-v") == 0)
            vFlag = 1;
        else if (strcmp(argv[i],"-r") == 0) {
            rrScheduling = 1;
            if (i+1 == argc) {
                printf("You must enter a time quantum if you want round robin scheduling," 
                    " e.g simcpu -r 10\n");
                exit(0);
            }
            timeQuantum = atol(argv[i+1]);
        }
    }
    firstProcess = malloc(sizeof(Process));
    //readyQueue->current = malloc(sizeof(Thread));
    //get starting line of info
    fscanf(stdin,"%d %d %d", &numProcesses, &switchThread, &switchProcess);
    int processesFinished[numProcesses];
    fscanf(stdin,"%d %d", &firstProcess->processNum, &firstProcess->numThreads);
    getThreads(firstProcess, firstProcess->numThreads, firstProcess->processNum);
    for (int i = 1; i < numProcesses; i++ ) {
        Process *nextProcess;
        nextProcess = malloc(sizeof(Process));
        fscanf(stdin,"%d %d", &nextProcess->processNum, &nextProcess->numThreads);
        getThreads(nextProcess, nextProcess->numThreads, nextProcess->processNum);
        //add process to linked list
        head = firstProcess;
        while (head->next != NULL)
            head = head->next;
        head->next = nextProcess;
    }
    //this loop runs until all threads in all processes are finished
    while (notFinished) {
        
        //go through threads and find arrival times that are lower
        //than clock time and add to queue, once queue is full, empty
        //queue and add cpu times to clock, refill queue with threads
        //that have arrived and are not in IO, repeat until finished
        head = firstProcess;
        //reinit ready queue
        readyQueue = malloc(sizeof(Queue));
        readyQueue->current = NULL;
        while (head != NULL) {
            for (int j = 0; j < head->numThreads; j++) {
                if (head->threads[j].arrivalTime <= clockTime && head->threads[j].IO_timeRemaining <= 0) {
                    if (head->threads[j].finishTime == 0) {
                        //if queue is empty
                        if (readyQueue->current == NULL) {
                            readyQueue->current = &head->threads[j];
                        }
                        else {
                            Qhead = readyQueue;
                            //this breaks for some stupid reason
                            while (Qhead->next != NULL) {
                                Qhead = Qhead->next;
                            }
                            Qnext = malloc(sizeof(Queue));
                            Qnext->current = &head->threads[j];
                            Qhead->next = Qnext;
                        }
                    }
                }
            }
            head = head->next;
        }

        //simulate chosen scheduling method
        if (rrScheduling == 0)
            simFCFS(readyQueue,&firstEvent,&currentProcess,&clockTime,switchProcess,switchThread);
        else if (rrScheduling == 1)
            //sim round robin
        
        //check finished
        head = firstProcess;
        i = 0;
        while (head != NULL) {
            processesFinished[i] = checkFinished(head, clockTime);
            i++;
            head = head->next;
        }
        for (int i = 0; i < numProcesses; i++) {
            if (processesFinished[i] == 0) {
                notFinished = 1;
                break;
            }
            else if (processesFinished[i] == 1) {
                notFinished = 0;
            }
        }
        readyQueue = NULL;
    }
    //print info based on flags
    if (dFlag == 0)
        printBasic(rrScheduling,timeQuantum,clockTime,switchTime);
    else if (dFlag == 1 || vFlag == 1) {
        printBasic(rrScheduling,timeQuantum,clockTime,switchTime);
        printDetailed(firstProcess);
    }

    return 0;
}

void getThreads(Process *process, int numThreads, int parent) {
    for (int i = 0; i < numThreads; i++) {
        fscanf(stdin,"%d %d %d", &process->threads[i].threadx, &process->threads[i].arrivalTime, &process->threads[i].bursts);
        process->threads[i].parentProcess = parent;
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

int checkFinished(Process *process, int newClockTime) {
    //this function also subtracts IO time from processes if needed
    static int oldClockTime = 0;
    int timeDifference = 0;
    //create array of size process->numThreads - 1, this will keep track of which 
    //threads are done
    int threadDone[process->numThreads-1];

    for (int i = 0; i < process->numThreads; i++) {
        threadDone[i] = 0;
    }

    for (int i = 0; i < process->numThreads; i++) {
        if (process->threads[i].finishTime != 0)
            threadDone[i] = 1;
        if (oldClockTime != newClockTime) {
            //subtract difference if thread is in IO
            timeDifference = newClockTime - oldClockTime;
            process->threads[i].IO_timeRemaining -= timeDifference;
            oldClockTime = newClockTime;
        }
    }
    //go through threadDone array and if all threads done return 1
    //else return zero to signal not done
    for (int i = 0; i < process->numThreads; i++) {
        if (threadDone[i] != 1)
            return 0;
    }
    return 1;
}

void simFCFS(Queue *readyQueue, int *firstEvent, int *currentProcess, int *clockTime, int switchProcess, int switchThread) {
    BurstTimes *temp;
    Queue *Qhead;

    //use a while loop to go through each thread in the queue
    while(readyQueue != NULL) {
        //The ready queue should now be full, empty queue and simulate time
        if (firstEvent) {
            currentProcess = &readyQueue->current->parentProcess;
            firstEvent = 0;
        }
        else {
            if (currentProcess == &readyQueue->current->parentProcess)
                *clockTime += switchThread;
            else
                *clockTime += switchProcess;
        }
        //add processing time to clockTime
        *clockTime += readyQueue->current->initBurst->CPU;
        readyQueue->current->serviceTime += readyQueue->current->initBurst->CPU;
        readyQueue->current->IO_timeRemaining = readyQueue->current->initBurst->IO;
        readyQueue->current->IOtime += readyQueue->current->initBurst->IO;
        //if the process is done, record finish time and calculate turnaround time
        if (readyQueue->current->initBurst->IO == 0) {
            readyQueue->current->finishTime = *clockTime;
            readyQueue->current->turnaroundTime = readyQueue->current->finishTime - 
                readyQueue->current->arrivalTime;
        }
        //move initburst to next burst
        if (readyQueue->current->initBurst->next != NULL) {
            temp = readyQueue->current->initBurst->next;
            free(readyQueue->current->initBurst);
            readyQueue->current->initBurst = temp;
        }
        //move queue to next thread
        if (readyQueue != NULL) {
            Qhead = readyQueue->next;
            free(readyQueue);
            readyQueue = Qhead;
        }
    }
    return;
}

void simRR() {
    
}

void printBasic(Process *firstProcess,int rrScheduling,int timeQuantum,int clockTime,int switchTime) {
    float utilization = 0;
    float averageTurnaround = 0;
    int totalThreads = 0;
    Process *head;

    if (rrScheduling == 1)
        printf("Round Robin Scheduling (quantum = %d time units)\n", timeQuantum);
    else
        printf("FCFS Scheduling\n");
    printf("Total Time required is %d time units\n", clockTime);
    head = firstProcess;
    while (head != NULL) {
        for (int i = 0; i < head->numThreads; i++) {
            averageTurnaround += head->threads[i].turnaroundTime;
            totalThreads++;
        }
        head = head->next;
    }
    averageTurnaround = averageTurnaround/totalThreads;
    printf("Average Turnaround Time is %0.1f time units\n", averageTurnaround);
    utilization = (switchTime/clockTime)*100;
    printf("CPU Utilization is %0.1f\n",utilization);
}

void printDetailed(Process *firstProcess) {
    Process *head;

    head = firstProcess;
    while (head != NULL) {
        for (int i = 0; i < head->numThreads; i++) {
            printf("Thread %d of Process %d:\n", head->threads[i].threadx, head->threads[i].parentProcess);
            printf("   arrival time: %d\n", head->threads[i].arrivalTime);
            printf("   service time: %d units, I/O time: %d units, turnaround time: %d units, finish time:"
                " %d units\n", head->threads[i].serviceTime, head->threads[i].IOtime, head->threads[i].turnaroundTime,
                head->threads[i].finishTime);
        }
        head = head->next;
    }
}

