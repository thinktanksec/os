#include <stdlib.h>
#include "semaphore.h"
#include "process.h"
#include "nios2_ctrl_reg_macros.h"



semaphoreCount = 0;
struct semaphore semaphoreArray[MAXSEM];

//==================
//Semaphore Functions

//Here we lay out memory for our new node.
void OS_InitSem(int sem_ID, int max_num)
{
	NIOS2_WRITE_STATUS(0);

	//Lay out memory for our semaphore
	struct semaphore newSem; //<-- DONT FUCKING MALLOC

	//Initialize some sem data
	newSem.semID = sem_ID;
	newSem.lock = max_num;
	newSem.waitListIndex = 0;
	newSem.next = NULL;

	int i;
	for(i = 0; i < MAXPROCESS; i++)
		newSem.waitingPID[i] = -16;

	for(i=0; i<MAXSEM; i++)
	{
		if(semaphoreArray[i].used == 0)
			break;
	}

	semaphoreArray[i].used = 1;
	semaphoreArray[i] = newSem;
	semaphoreCount++;

	NIOS2_WRITE_STATUS(1);
}




void OS_Wait(int sem_ID)
{
	NIOS2_WRITE_STATUS(0);
	asm("mov %0, r31" : "=r"(currentProc.registers[31]));
	
	//Find the sem_ID
	int i;
	for(i=0; i < MAXSEM; i++)
	{
		if(semaphoreArray[i].semID == sem_ID);
			break;
	}

	//decrement lock
	semaphoreArray[i].lock -= 1;
	
	if(semaphoreArray[i].lock < 0)
	{
		printf("Got locked out\n");
		//Set current process lock.
		currentProc.procLock = 1;

		//Add the caller to the list of waiting processes.
		semaphoreArray[i].waitingPID[semaphoreArray[i].waitListIndex] = currentProc.pid;
		semaphoreArray[i].waitListIndex++;

		//Check if lock is in use
		//printf("Bout ta lock");
		while(currentProc.procLock = 1)
		{
			asm("trap");
			asm("ret");
		}
	}

	//printf("Got lock no prob");
	NIOS2_WRITE_STATUS(1);
	//asm("ret");
}




void OS_Signal(int sem_ID)
{
	NIOS2_WRITE_STATUS(0);
	//printf("signal was called");
	//Find the sem_ID
	int i;
	for(i=0; i < MAXSEM; i++)
	{
		if(semaphoreArray[i].semID == sem_ID);
			break;
	}

	//Gather the PId of the next process up
	//for the semaphore.
	PID nextProcessPID = semaphoreArray[i].waitingPID[0];

	//Move through process list until we find
	//the one that matches and tell it to go.
    for(i=0; i < MAXPROCESS; i++)
    {
    	if(processArray[i].pid == nextProcessPID)
    		break;
    }

    //Give that process the go ahead.
    processArray[i].procLock = 0;

    //Remove the process from the wait list
    for(i = 0; i < MAXPROCESS; i++)
		semaphoreArray[i].waitingPID[i] = semaphoreArray[i].waitingPID[i+1];
    
	//Update index
	semaphoreArray[i].waitListIndex--;

	//increment lock
	semaphoreArray[i].lock += 1;
	
	NIOS2_WRITE_STATUS(1);
}


