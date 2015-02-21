#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SPORADIC           2


//====================
//GLOBALS
typedef unsigned int PID;
PID pidIndex = 0;


//=================
//Structs
typedef struct proc_t
{
	//Used to ID the process
	PID PID; //<---- :o

	//Function Handle
	int (* process)();

    //Lock

    //Argument
    int arg;

	//Traverse the doubly linked list.
	struct Node* next;
	struct Node* prev;
}process;




//Head of our main process list.
process* sporadicQueueHead = NULL;





int processFunction(void)
{
	puts("buttsbuttsbuttsbutts");
}











//----------------------------------------------------------------
//						PROCESS CALLS
//----------------------------------------------------------------



//==================
//Queue Functions
/*
 * The semaphores work as a giant queue. The masterSem is the master head of the semaphore queue.
 * Each semaphore that is created gets appended to the cue that masterSem is the head of.
 */
int enqueueProcess(process **head, process *newProcess)
{
    process *tmp;

    if (newProcess == NULL)
        return -1;

    //Case where it is the first one in the list.
    if (*head == NULL)
    {
        *head = newProcess;
        newProcess->next = NULL;
    }
    //Else the list is not empty, append this sem to the tail.
    else
    {
        tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = newProcess;
        newProcess->next = NULL;
    }
    return 1;
}




process* dequeueProcess(process **head)
{
    process *tmp = NULL;

    if (*head != NULL)
    {
        tmp = *head;
        *head = (*head)->next;
    }
    else
    {
    	return NULL;
    }
    return tmp;
}




//Given a process queue, it will find the matching process
//for a given PID
process* findPID(process *head, int PID)
{
    process *tmp = head;
    while (tmp != NULL)
        if (tmp->PID == PID)
            return tmp;
        else
            tmp = tmp->next;
    return tmp;
}





PID OS_Create(void (*f)(void), int arg, unsigned int level, unsigned int n)
{
    pidIndex += 1;

    //Allocate and create
    process *proc = malloc(1024);

    //Set some fields
    proc->process = f;
	proc->PID = pidIndex;
    proc->arg = arg;

	//Append to the appropriate queue
	enqueueProcess(&sporadicQueueHead, proc);

	//Set the function pointer.

	return pidIndex;
}



void OS_Terminate(void)
{
	//Get the process off the queue.
	process *proc = dequeueProcess(&sporadicQueueHead);

	//Kill it.

	//Release semaphores it had

	//Free up its memory.




}


void OS_Yield(void)
{
	//Queue the head process into the back.
}


int  OS_GetParam(void)
{
	process *proc = sporadicQueueHead;

	int param = proc->arg;

	return param;
}


//----------------------------------------------------------------
//						PROCESS CALLS
//----------------------------------------------------------------














void OS_Init()
{
 	//SHOULD CREATE A INIT PROCESS.
}



void OS_Start()
{
	//Scheduling loop
	while(1)
	{
		puts("Making first process");
		//Making first process
		//==========================

		//Create function pointer.
    	void (*foo)(void);
   		foo = processFunction;
		PID proc1 = OS_Create(foo, 2, SPORADIC, 0);
		PID proc2 = OS_Create(foo, 2, SPORADIC, 0);


		
		
		
		//SCHEDULING
		
		
		while(sporadicQueueHead != NULL)
		{
			puts("dequeue first process");
			//Pop a process
			process *proc = dequeueProcess(&sporadicQueueHead);

			//Execute its function pointer.
			proc->process();




			puts("dequeue first process");
			//Pop a process
			process *proc2 = dequeueProcess(&sporadicQueueHead);

			//Execute its function pointer.
			proc2->process();
		}
	}
}



void OS_Abort()
{

}




void main()
{

	//OS_Init();


	OS_Start();
}
