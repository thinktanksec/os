#include "myos.h"

//=================
//Structs

struct semaphore
{
	//Used to ID the sem
	int semID;

	//The position of this semaphore in the
	//master active semaphore array.
	int index;

	//The actual mutex value.
	int lock;

	//Indicated if it is used or not.
	int used;

	//The array of waiting PIDs
	PID waitingPID[MAXPROCESS];
	int waitListIndex;

	//For semaphore queue
	struct sem_t* next;
};

struct semaphore semaphoreArray[];


//===================
//GLOBALS
extern int semaphoreCount;

