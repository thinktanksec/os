#include "process.h"
#include "nios2_ctrl_reg_macros.h"
#define NULL 0
int pidGen = 100;
sporadicCount = -1;
deviceCount = -1;
struct processStruct processArray[MAXPROCESS];
struct processStruct sporadicQueue[15];
struct processStruct deviceQueue[15];

PID OS_Create(void (*f)(void), int arg, unsigned int level, unsigned int n) 
{	
	NIOS2_WRITE_STATUS(0);

	if(MAXPROCESS - processCount == 0)
		return -2; //<-- Could not service request.

	int i;
	struct processStruct newTask;
	
	//Set process parameters
	newTask.pid = pidGen;
	newTask.state = NEW;
	newTask.name = n;
	newTask.level = level;
	newTask.process = f;
	newTask.arg = arg;
	newTask.next = NULL;
	newTask.done = 0;
	newTask.procLock = 0;


	//Set Execution Time
	if(level != DEVICE)
	{
		int index;
		for(i = 0; i < MAXPROCESS; i++)
		{
			if(PPP[i] == newTask.name)
			{
				index = i;
				break;
			}
		}
		newTask.execTime = PPPMax[index];
	}
	else if(level == DEVICE)
		newTask.execTime = n;
	


	//Set the register array to zero
	for(i = 0; i < 31; i++)
		newTask.registers[i] = 0;
	
	

	//Register the new process in the process table
	if(level == PERIODIC)
	{
		i=0;
		while (processArray[i].pid != -2) i++;
		processArray[i] = newTask;
	}
	

	if(level == SPORADIC)
	{
		//This is the case if this is the first sporadic process.
		if(sporadicCount == -1)
			sporadicCount = 0;

		sporadicCount++;
		i=0;
		while (sporadicQueue[i].pid != -2) i++;
		sporadicQueue[i] = newTask;
	}


	if(level == DEVICE)
	{
		//This is the case if this is the first sporadic process.
		if(deviceCount == -1)
			deviceCount = 0;

		deviceCount++;
		i=0;
		while (deviceQueue[i].pid != -2) i++;
		deviceQueue[i] = newTask;
	}


	//Update process counts
	processCount++;
	pidGen++;

	//printf("MADE NEW PROC: [%d]\n", processCount);
	NIOS2_WRITE_STATUS(1);
	return newTask.pid;
}



void OS_Terminate() 
{
	NIOS2_WRITE_STATUS(0);
	printf("Process %d is done [%d]\n", currentProc.name, currentProc.state);
	//Update process struct
	currentProc.state = TERMINATED;
	
	//Update process counts.
	processCount--;      

	if(currentProc.level == SPORADIC)
		sporadicCount--;

	if(currentProc.level == DEVICE)
		deviceWaiting = 0;

	//printf("PROCESS COUNT [%d] SPORADIC COUNT[%d]\n", processCount, sporadicCount);
	if(processCount <= 0)
	{
		NIOS2_WRITE_STATUS(0);
		printf("I'm fucking aborting NOW");
		NIOS2_RESET();
	}
	else
	{
		asm ("movia r2, offToDeath");
		asm ("jmp r2");
	}
}  



void OS_Yield(void) 
{
	NIOS2_WRITE_STATUS(0);
	//puts("called yield");
	currentProc.yield = 1;
	//stopTimer();
	//NIOS2_WRITE_STATUS(1);

	//asm ("movia r2, the_exception");
	//asm ("jmp r2");
	unsigned int temp;
	asm("mov %0, r31" : "=r"(currentProc.registers[31]));
	printf("Process [%d] rRA [%#010x]\n", currentProc.name, currentProc.registers[31]);

	asm("trap");
	
	asm("ret");
}


int OS_GetParam(void) {
	NIOS2_WRITE_STATUS(0);
	NIOS2_WRITE_STATUS(1);
	return currentProc.arg;
}



//Given a process queue, it will find the matching process
//for a given name
struct processStruct findName(int name)
{
    int q = 0;
    while(processArray[q].name != name)
        q++;

    return processArray[q];
}
