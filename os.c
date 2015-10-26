#include "myos.h"
#include "memory.h"
#include "process.h"
#include "semaphore.h"
#include "FIFO.h"
#include "nios2_ctrl_reg_macros.h"
#include <stdio.h>
#define NULL 0

#define _0	0x3F
#define _1	0x06 
#define _2	0x5B
#define _3	0x4F
#define _4	0x66
#define _5	0x6D
#define _6	0x7D
#define _7	0x07
#define _8	0x7F
#define _9	0x6F

#define NOTEBOOKSIZE 3

#define GREEN_LED_BASE 0x10000010

volatile char *HEX3_HEX0 = (char*) 0x10000020;
volatile char *HEX7_HEX4 = (char*) 0x10000030;

//====================
//		GLOBALS
//====================
int pppIndex = 0;
int PPP[PPPLen] = {IDLE, 1};
int PPPMax[];   
int started = 0;
FIFO fifo1;
int x = 0;
int q = 0;
int deviceTimer = 0;
deviceWaiting = 0;




//==========================================
//			YOUR PROGRAM GOES HERE
//==========================================

//=======================================================
//			"TEST APPLICATION" functions
//=======================================================
void idle(void)
{
	//If the idle process ever gets to execute its own 
	//function, it means that we have run out of non terminated
	//sporadic processes for the scheduler to switch to. So the
	//original switch to the IDLE process before we look for a 
	//sporadic will hold and we simply terminate it.
	NIOS2_WRITE_STATUS(0);
	printf("hit the idle process\n");
	NIOS2_WRITE_STATUS(1);

	//this is the case when the sporadic queue is empty
	//when we start the program. Once a sporadic process
	//is creadted it will increment it to reflect the appropriate
	//ammount of sporadic processes then this idle process will wait until
	//they are all complete. If none are made then we check the case
	//that the idle process is the last one in the system 
	while(sporadicCount == -1);

	while(sporadicCount != 0);
	OS_Terminate();
}


void processFunction2(void)
{
	x = 5;
	OS_Wait();
	NIOS2_WRITE_STATUS(0);
	printf("Swaped X\n");
	NIOS2_WRITE_STATUS(1);


	OS_Terminate();
}

void blink_pattern(){

	NIOS2_WRITE_STATUS(0);

	volatile int * green_led_ptr = (int*) GREEN_LED_BASE;
	
	unsigned int green_led_pattern;
	unsigned int pattern;
	green_led_pattern = 0x55555555;
	pattern = green_led_pattern;
	
	int x = 0;
	int count;
	int count2;
	while(x < 100){
		for(count = 0; count < 100000; count++){
			*(green_led_ptr) = pattern;	
		}
		for(count2 = 0; count2 < 100000; count2++){
			*(green_led_ptr) = 0;
		}
		x++;
	}
	NIOS2_WRITE_STATUS(1);
	return;

}







//----------------------------------------------------------------
//						   OS FUNCTIONS
//----------------------------------------------------------------
void OS_Init()
{
    //------------------
    //		MEMORY
    //------------------
    //Setup all the memory structs and zero out the heap
    OS_InitMemory();

    int i;
    //Set all PCBs to available.
    for(i = 0; i < MAXPROCESS; i++)
    	processArray [i].pid = -2;

    //Set all FIFOs to available
    for(i = 0; i < MAXFIFO; i++)
    	fifoArray[i].used = 0;

    //Set all Semaphores to available
    for(i = 0; i < MAXSEM; i++)
    	semaphoreArray[i].used = 0;

    //Set all sporadic processes to available
    for(i = 0; i < 15; i++)
    	sporadicQueue[i].pid = -2;

    //Set all device processes to available
    for(i = 0; i < 15; i++)
    	deviceQueue[i].pid = -2;


    setPPP();
	
    PID proc0 = OS_Create(idle, IDLE, PERIODIC, IDLE);
    PID proc2 = OS_Create(class1, 1, PERIODIC, 1);

}





void setPPP()
{
	//---------------------
	//		Just PPP
	//---------------------
	//Init the PPP           
	PPPMax[PPPLen];     

	//Set the time for each PPP in their arrays. 
	PPPMax[0] = 3;
	PPPMax[1] = 1;
}



void kernel() __attribute__ ((section (".kernel")));
void kernel()
{
	////printf("I AM PROCESS %d\n", currentProc->name);
	currentProc.state = RUNNING;

	if(currentProc.level != DEVICE)
	{
		startTimer(currentProc.execTime);
		NIOS2_WRITE_STATUS(1);
	}
	
	currentProc.process();
}


void offToDeath()
{
	//If a "program" gets here they are done so make them wait forever.
	NIOS2_WRITE_STATUS(0);
	printf("Process %d is done [%d]\n", currentProc.name, currentProc.state);
	NIOS2_WRITE_STATUS(1);
	
	//Get a quick check in before we set interrupt enable again.
	if(processCount <= 0)
	{
		NIOS2_WRITE_STATUS(0);
		printf("DIEDIEDIE\n");
		NIOS2_WRITE_STATUS(1);
		OS_Abort();
	}

	//Just wait it out.
	NIOS2_WRITE_STATUS(1);
	while(1)
	{
		if(processCount <= 0)
		{
			NIOS2_WRITE_STATUS(0);
			OS_Abort();
		}
	}

	OS_Abort();
}




void OS_Start()
{
	kernel();
}



void OS_Abort()
{
	NIOS2_WRITE_STATUS(0);
	*(HEX3_HEX0) 	 = _0;
	*(HEX3_HEX0 + 1) = _0;
	*(HEX3_HEX0 + 2) = _0;
	*(HEX3_HEX0 + 3) = _0;
	*(HEX7_HEX4) 	 = _0;
	*(HEX7_HEX4 + 1) = _0;
	*(HEX7_HEX4 + 2) = _0;
	*(HEX7_HEX4 + 3) = _0;
	
	printf("I'm fucking aborting NOW\n");
	NIOS2_RESET();
	//NIOS2_WRITE_STATUS(1);
}

void OS_Abort2()
{
	NIOS2_WRITE_STATUS(0);
	printf("I have been reset\n");
	OS_Abort();
	//NIOS2_RESET();
}

void main()
{
	NIOS2_WRITE_STATUS(0);  //<----------- DISABLE INTERRUPTS
	NIOS2_WRITE_IENABLE(0x3);  
	if(started == 0)
	{
		OS_Init();
		schedule();
		started = 1;
	}

	OS_Start();
}





//----------------------------------------------------------------
//						  END OS FUNCTIONS
//----------------------------------------------------------------
int schedule()
{
	NIOS2_WRITE_STATUS(0);
	printf("SCHEDULE\n");
	//Change the current PPP
	//We use Modular division to create a circular bounded buffer.
	int indexMod = pppIndex % PPPLen;
	int tmpName = PPP[indexMod];

	currentProc = findName(tmpName);

	if(pppIndex >= PPPLen)
		pppIndex = 1;
	else
		pppIndex++;

	if(currentProc.state == TERMINATED){
		printf("*We Hit a TERM PROC [%d]\n", currentProc.name);
	}


	if(currentProc.name == IDLE) //WE found the idle process
	{
		//Find our first sporadic process.
		int i;
		for(i = 0; i < sporadicCount+1; i++)
		{
			if(sporadicQueue[i].state != TERMINATED)
			{
				printf("SPORADIC PROCESS %d is: [%d][%d][%d][%d]\n", i, sporadicQueue[i].name, 
					sporadicQueue[i].level, sporadicQueue[i].state, sporadicCount);

				currentProc = sporadicQueue[i];
				break;
			}
		}

		if(sporadicCount == 0)
			currentProc = findName(tmpName);
	}



	//Device Level Manager
	if(deviceCount > -1) //<-- Denotes that there is a device process.
	{
		//if no device then set a new device.
		if(deviceWaiting == 0)
		{
			deviceWaiting = 1;
			int i;
			for(i = 0; i < deviceCount+1; i++)
			{
				if(deviceQueue[i].name != 0 && deviceQueue[i].state != TERMINATED)
				{
					printf("DEVICE PROCESS %d is: [%d][%d][%d][%d]\n", i, deviceQueue[i].name,
						deviceQueue[i].level, deviceQueue[i].state, deviceCount);

					currentDevice = deviceQueue[i];
					deviceTimer = currentDevice.execTime;
					break;
				}
			}
		}

		//Decrement the device time to see if we need to run it.
		deviceTimer -= currentProc.execTime;
		printf("DeviceTimer [%d]\n", deviceTimer);
		if(deviceTimer <= 0)
		{
			printf("given execution to the device process\n");
			currentProc = currentDevice;
			deviceTimer = 10000;
		}
	}


	NIOS2_WRITE_STATUS(1);
	return 0;
}



void the_reset(void)__attribute__((section(".reset")));
void the_reset (void)
{
	asm ("movia r2, OS_Abort2");
	asm ("jmp r2");
}



void the_exception(void)__attribute__((section(".exceptions")));
void the_exception(void)
{
	//Save Registers 
	if(currentProc.state == RUNNING)
	{
		if(currentProc.yield == 0 && currentProc.procLock == 0)
			asm("mov %0, r31" : "=r"(currentProc.registers[31]));

		currentProc.state = WAITING;
		MEMORY stackPointer;

		asm("mov %0, r1" : "=r"(currentProc.registers[1]));
		asm("mov %0, r2" : "=r"(currentProc.registers[2]));
		asm("mov %0, r3" : "=r"(currentProc.registers[3]));
		asm("mov %0, r4" : "=r"(currentProc.registers[4]));
		asm("mov %0, r5" : "=r"(currentProc.registers[5]));
		asm("mov %0, r6" : "=r"(currentProc.registers[6]));
		asm("mov %0, r7" : "=r"(currentProc.registers[7]));
		asm("mov %0, r8" : "=r"(currentProc.registers[8]));
		asm("mov %0, r9" : "=r"(currentProc.registers[9]));
		asm("mov %0, r10" : "=r"(currentProc.registers[10]));
		asm("mov %0, r11" : "=r"(currentProc.registers[11]));
		asm("mov %0, r12" : "=r"(currentProc.registers[12]));
		asm("mov %0, r13" : "=r"(currentProc.registers[13]));
		asm("mov %0, r14" : "=r"(currentProc.registers[14]));
		asm("mov %0, r15" : "=r"(currentProc.registers[15]));
		asm("mov %0, r16" : "=r"(currentProc.registers[16]));
		asm("mov %0, r17" : "=r"(currentProc.registers[17]));
		asm("mov %0, r18" : "=r"(currentProc.registers[18]));
		asm("mov %0, r19" : "=r"(currentProc.registers[19]));
		asm("mov %0, r20" : "=r"(currentProc.registers[20]));
		asm("mov %0, r21" : "=r"(currentProc.registers[21]));
		asm("mov %0, r22" : "=r"(currentProc.registers[22]));
		asm("mov %0, r23" : "=r"(currentProc.registers[23]));
		asm("mov %0, r24" : "=r"(currentProc.registers[24]));
		asm("mov %0, r25" : "=r"(currentProc.registers[25]));
		asm("mov %0, r26" : "=r"(currentProc.registers[26]));

		asm("mov %0, r27" : "=r"(stackPointer));
		currentProc.sp = stackPointer;

		asm("mov %0, r28" : "=r"(currentProc.registers[28]));
		asm("mov %0, r29" : "=r"(currentProc.registers[29]));
		asm("mov %0, r30" : "=r"(currentProc.registers[30]));


		//printf("Saved Registers");
		printf("Process [%d] rEA [%#010x]\n", currentProc.name, currentProc.registers[29]);
		printf("Process [%d] rRA [%#010x]\n", currentProc.name, currentProc.registers[31]);


		//Replace process table entry.
		if(currentProc.level == PERIODIC)
		{
			int i;
			for(i=0; i<MAXPROCESS; i++)
			{
				if(processArray[i].pid == currentProc.pid)
				{
					processArray[i] = currentProc;
					break;
				}
			}
		}

		if(currentProc.level == SPORADIC)
		{
			int i;
			for(i=0; i<MAXPROCESS; i++)
			{
				if(sporadicQueue[i].pid == currentProc.pid)
				{
					sporadicQueue[i] = currentProc;
					break;
				}
			}
		}

		if(currentProc.level == DEVICE)
		{
			int i;
			for(i=0; i<MAXPROCESS; i++)
			{
				if(deviceQueue[i].pid == currentProc.pid)
				{
					deviceQueue[i] = currentProc;
					break;
				}
			}
		}
	}
	else if(currentProc.state == TERMINATED)
	{
		if(currentProc.done == 0)
		{
			currentProc.done = 1;
			MEMORY stackPointer;

			asm("mov %0, r1" : "=r"(currentProc.registers[1]));
			asm("mov %0, r2" : "=r"(currentProc.registers[2]));
			asm("mov %0, r3" : "=r"(currentProc.registers[3]));
			asm("mov %0, r4" : "=r"(currentProc.registers[4]));
			asm("mov %0, r5" : "=r"(currentProc.registers[5]));
			asm("mov %0, r6" : "=r"(currentProc.registers[6]));
			asm("mov %0, r7" : "=r"(currentProc.registers[7]));
			asm("mov %0, r8" : "=r"(currentProc.registers[8]));
			asm("mov %0, r9" : "=r"(currentProc.registers[9]));
			asm("mov %0, r10" : "=r"(currentProc.registers[10]));
			asm("mov %0, r11" : "=r"(currentProc.registers[11]));
			asm("mov %0, r12" : "=r"(currentProc.registers[12]));
			asm("mov %0, r13" : "=r"(currentProc.registers[13]));
			asm("mov %0, r14" : "=r"(currentProc.registers[14]));
			asm("mov %0, r15" : "=r"(currentProc.registers[15]));
			asm("mov %0, r16" : "=r"(currentProc.registers[16]));
			asm("mov %0, r17" : "=r"(currentProc.registers[17]));
			asm("mov %0, r18" : "=r"(currentProc.registers[18]));
			asm("mov %0, r19" : "=r"(currentProc.registers[19]));
			asm("mov %0, r20" : "=r"(currentProc.registers[20]));
			asm("mov %0, r21" : "=r"(currentProc.registers[21]));
			asm("mov %0, r22" : "=r"(currentProc.registers[22]));
			asm("mov %0, r23" : "=r"(currentProc.registers[23]));
			asm("mov %0, r24" : "=r"(currentProc.registers[24]));
			asm("mov %0, r25" : "=r"(currentProc.registers[25]));
			asm("mov %0, r26" : "=r"(currentProc.registers[26]));

			asm("mov %0, r27" : "=r"(stackPointer));
			currentProc.sp = stackPointer;

			asm("mov %0, r28" : "=r"(currentProc.registers[28]));
			asm("mov %0, r29" : "=r"(currentProc.registers[29]));
			asm("mov %0, r30" : "=r"(currentProc.registers[30]));
			asm("mov %0, r31" : "=r"(currentProc.registers[31]));

			//printf("Saved Registers");
			printf("Process [%d] rEA [%#010x]\n", currentProc.name, currentProc.registers[29]);
			printf("Process [%d] rRA [%#010x]\n", currentProc.name, currentProc.registers[31]);

			//Replace process table entry.
			if(currentProc.level == PERIODIC)
			{
				int i;
				for(i=0; i<MAXPROCESS; i++)
				{
					if(processArray[i].pid == currentProc.pid)
					{
						processArray[i] = currentProc;
						break;
					}
				}
			}

			if(currentProc.level == SPORADIC)
			{
				int i;
				for(i=0; i<MAXPROCESS; i++)
				{
					if(sporadicQueue[i].pid == currentProc.pid)
					{
						sporadicQueue[i] = currentProc;
						break;
					}
				}
			}

			if(currentProc.level == DEVICE)
			{
				int i;
				for(i=0; i<MAXPROCESS; i++)
				{
					if(deviceQueue[i].pid == currentProc.pid)
					{
						deviceQueue[i] = currentProc;
						break;
					}
				}
			}
		}
		//else


		//NIOS2_WRITE_STATUS(1);
	}	




	asm ("call INTERRUPT_HANDLER");




	if (currentProc.state == WAITING)
	{
		currentProc.state = RUNNING;

		if(currentProc.yield == 1)
			currentProc.yield = 0;
		
		printf("loading RAGISTERS\n");
		MEMORY stackPointer;
		stackPointer = currentProc.sp;

		if(currentProc.level != DEVICE)
			startTimer(currentProc.execTime);

		asm("mov r1, %0" :: "r"(currentProc.registers[1]));
		asm("mov r2, %0" :: "r"(currentProc.registers[2]));
		asm("mov r3, %0" :: "r"(currentProc.registers[3]));
		asm("mov r4, %0" :: "r"(currentProc.registers[4]));
		asm("mov r5, %0" :: "r"(currentProc.registers[5]));
		asm("mov r6, %0" :: "r"(currentProc.registers[6]));
		asm("mov r7, %0" :: "r"(currentProc.registers[7]));
		asm("mov r8, %0" :: "r"(currentProc.registers[8]));
		asm("mov r9, %0" :: "r"(currentProc.registers[9]));
		asm("mov r10, %0" :: "r"(currentProc.registers[10]));
		asm("mov r11, %0" :: "r"(currentProc.registers[11]));
		asm("mov r12, %0" :: "r"(currentProc.registers[12]));
		asm("mov r13, %0" :: "r"(currentProc.registers[13]));
		asm("mov r14, %0" :: "r"(currentProc.registers[14]));
		asm("mov r15, %0" :: "r"(currentProc.registers[15]));
		asm("mov r16, %0" :: "r"(currentProc.registers[16]));
		asm("mov r17, %0" :: "r"(currentProc.registers[17]));
		asm("mov r18, %0" :: "r"(currentProc.registers[18]));
		asm("mov r19, %0" :: "r"(currentProc.registers[19]));
		asm("mov r20, %0" :: "r"(currentProc.registers[20]));
		asm("mov r21, %0" :: "r"(currentProc.registers[21]));
		asm("mov r22, %0" :: "r"(currentProc.registers[22]));
		asm("mov r23, %0" :: "r"(currentProc.registers[23]));
		asm("mov r24, %0" :: "r"(currentProc.registers[24]));
		asm("mov r25, %0" :: "r"(currentProc.registers[25]));
		asm("mov r26, %0" :: "r"(currentProc.registers[26]));

		asm("mov sp, %0" :: "r"(stackPointer)); 

		asm("mov r28, %0" :: "r"(currentProc.registers[28]));
		asm("mov r29, %0" :: "r"(currentProc.registers[29]));
		asm("mov r30, %0" :: "r"(currentProc.registers[30]));
		asm("mov r31, %0" :: "r"(currentProc.registers[31]));

		asm ("eret");
	}

	if(currentProc.state == NEW)
	{
		asm ("movia r2, kernel");
		asm ("jmp r2");
	}
}


void INTERRUPT_HANDLER()
{
	int ipending, ea_addr;
	NIOS2_READ_IPENDING(ipending);

	if (ipending & 0x01)	
	{
		stopTimer();
		schedule();	
	}
	else if(currentProc.yield == 1)
	{
		stopTimer();
		schedule();
	}
	else if(currentProc.procLock == 1)
	{
		stopTimer();
		schedule();
	}
}


void startTimer(int msec) 
{
	volatile int * interval_timer_ptr = (int *) 0x10002000;	

	int counter = 50000 * msec;				// 1/(50 MHz) x (0x190000) = 33 msec
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;
	*(interval_timer_ptr + 1) = 0x7;
}




void stopTimer() 
{
	volatile int * interval_timer_ptr = (int *) 0x10002000;
	*(interval_timer_ptr) = 0; 
}



void saveRegisters() 
{
}



void loadRegisters() 
{
	printf("loading registers");
}

