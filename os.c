//Board Includes.
//#include <stdlib.h>
#include "context_switch.h"
#include "nios2_ctrl_reg_macros.h"
#include "nios2.h"

//#include "os.h"

#define SPORADIC           2

#define NULL               0


//===================================================
// Macro for resetting the board!
//===================================================
#define NIOS2_RESET_ADDR 0x00900800
#define NIOS2_RESET() \
	NIOS2_WRITE_STATUS(0);\
	NIOS2_WRITE_IENABLE(0);\
	((void (*) (void)) NIOS2_RESET_ADDR)()


//====================
// OS GLOBALS
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


	//Has it completed its function.
	int functionComplete;

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
	volatile int * red_LED_ptr 	= (int *) 0x10000000;	// red LED address
	volatile int * SW_switch_ptr	= (int *) 0x10000040;	// SW slider switch address
	int SW_value, KEY_value;


	SW_value = *(SW_switch_ptr);		 	// read the SW slider switch values
	*(red_LED_ptr) = SW_value; 			// light up the red LEDs

	if(SW_value == 0)
	{
		while(SW_value != 131072)
		{
			SW_value = *(SW_switch_ptr);		 	// read the SW slider switch values
			*(red_LED_ptr) = SW_value; 			// light up the red LEDs
		}
	}
	else if(SW_value == 131072)
	{
		while(SW_value != 0)
		{
			SW_value = *(SW_switch_ptr);		 	// read the SW slider switch values
			*(red_LED_ptr) = SW_value; 			// light up the red LEDs
		}
	}
	else
	{
		while(SW_value != 131072)
		{
			SW_value = *(SW_switch_ptr);		 	// read the SW slider switch values
			*(red_LED_ptr) = SW_value; 			// light up the red LEDs
		}
	}

	process *proc = sporadicQueueHead;
	proc->functionComplete = 1;
		
	return 1;
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



//----------------------------------------------------------------
//					    PROCESS PRIMITIVES
//----------------------------------------------------------------

PID OS_Create(void (*f)(void), int arg, unsigned int level, unsigned int n)
{
    pidIndex += 1;

    //Allocate and create
    process *proc = malloc(1024);

    //Set some fields
    proc->process = f;
	proc->PID = pidIndex;
    proc->arg = arg;
    proc->functionComplete = 0;

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
	
	// TODO: When multiple processes are implemented (periodic, device),
	//		 implement OS_Yield.
	//
	//		 Do this by dropping the current process in progress and 
	//		 return it to the end of the queue to try again at a later
	//		 point. This will return the context switch back to the
	//		 scheduler, to prevent dealing with multiple scope levels.
	// 		 :D
	
}

int  OS_GetParam(void)
{
	process *proc = sporadicQueueHead;
	int param = proc->arg;

	return param;
}



//----------------------------------------------------------------
//					     OS INITIALIZATION
//----------------------------------------------------------------

void OS_Init()
{
 	//Making first process
	//==========================
	//Create function pointer.
    puts("Making first process");
    void (*foo)(void);
   	foo = processFunction;
	PID proc1 = OS_Create(foo, 2, SPORADIC, 0);
	
	//------------------------------------------------
	// These are not relevant until we implement
	// more than just sporadic processes.
	//------------------------------------------------
	// TODO: initialize PPP[]
	// TODO: initialize PPPMax[]
	
	//OS_InitSem();
	//OS_InitFiFo();
}

void OS_Start()
{
		
	//SCHEDULING
	while(1)
	{
		//Pop a process
		puts("Set the head pointer.");
		process *currentProc = sporadicQueueHead;


		//========================
		//Handle interrupt

		the_reset();

		volatile int * interval_timer_ptr = (int *) 0x10002000;	// interval timer base address

		/* set the interval timer period for scrolling the HEX displays */
		int counter = 0x190000;				// 1/(50 MHz) x (0x190000) = 33 msec
		*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
		*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

		/* start interval timer, enable its interrupts */
		*(interval_timer_ptr + 1) = 0x7;	// STOP = 0, START = 1, CONT = 1, ITO = 1 
		
		NIOS2_WRITE_IENABLE( 0x3 );	
		NIOS2_WRITE_STATUS( 1 );

		the_exception();

		// END Handle interrupt
		//========================		

		
		//Wait for the interrupt somehow.
		while(currentProc->functionComplete != 1);

		puts("We returned from the context switch.");
		
		//Throw the process to the back of the list.
		process *temp = dequeueProcess(&sporadicQueueHead);
		enqueueProcess(&sporadicQueueHead, temp);


		/* 
		* Now we wait for the timer on the board to finish and
		* generate another interrupt?
		*/
	}
}

void OS_Abort()
{
	NIOS2_RESET();
}

//----------------------------------------------------------------
//						 	   MAIN
//----------------------------------------------------------------

/*

	This is what the Altera Monitor program loads+runs.
	This function is fundamental to making the OS run.

*/
void main()
{
	OS_Init();

	OS_Start();
}


//----------------------------------------------------------------
//						 ? BOARD FUNCTIONS ?
//----------------------------------------------------------------

// ?????
// What goes here?
// ?????
