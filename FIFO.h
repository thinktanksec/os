#include "myos.h"


//================================================
//				 STOP INTERRUPTS
//================================================
//=================
//Structs

struct fifoStruct
{
	//Holds the FIFO descriptor used to
	//ID this particular FIFO.
	FIFO descriptor;

	//The current position of the writer
	int indexW;

	//The current position of the reader.
	int indexR;

	//Is it used?>??
	int used;

	//Pointer to the first array element of fifo data.
	int data[FIFOSIZE];

	//Linked List stuff
	//Traverse the doubly linked list.
	struct FIFOstruct* next;
	struct FIFOstruct* prev;

};
struct fifoStruct fifoArray[];


//===================
//GLOBALS
extern int fifoCount;
