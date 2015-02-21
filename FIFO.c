#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXFIFO            16     /* max. # of FIFOs supported */
#define FIFOSIZE           8      /* max. # of data elements per FIFO */
#define INVALIDFIFO        0      /* an invalid FIFO descriptor */

#define TRUE               1	  /* Boolean */
#define FALSE              0      /* Boolean */

typedef unsigned int BOOL;
typedef unsigned int FIFO;


/*

	TODO: HANDLE WRITES OVERTAKING READS

*/


//================================================
//				 STOP INTERRUPTS
//================================================
//=================
//Structs

typedef struct FIFOstruct
{
	//Holds the FIFO descriptor used to
	//ID this particular FIFO.
	FIFO descriptor;

	//The current position of the writer
	int indexW;

	//The current position of the reader.
	int indexR;

	//Pointer to the first array element of fifo data.
	unsigned int data[FIFOSIZE];
}FIFOstruct;


//===================
//GLOBALS
FIFOstruct masterFIFOArray[MAXFIFO];


FIFO OS_InitFiFo()
{
	//Create some elements to build the new FIFO
	FIFOstruct newFIFO;

	//Make a unique descriptor (the address of the first element.)
	newFIFO.descriptor = newFIFO.data; //WARNING: makes int from ptr wo cast
	newFIFO.indexW = 0;
	newFIFO.indexR = 0;

	//initialize every element in the buffer to NULL
	int i;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		newFIFO.data[i] = NULL; //WARNING: makes int from ptr wo cast
	}

	//Parse the master array, find a spot for new FIFO
	for(i = 0; i < MAXFIFO -1; i++)
	{
		if(masterFIFOArray[i] == NULL)
		{
			masterFIFOArray[i] = newFIFO.descriptor;
			newFIFO.indexW = i;
			return newFIFO.descriptor;
		}
	}

	
	//Return INVALIDFIFO if no valid FIFO descriptor available
	return INVALIDFIFO;
}

/*
  Write will always succeed.
  If FIFO full, first unread is dropped.
  
  TODO: WE AREN'T DEALING WITH f AT ALL!
	We check if temp = f... that's it
*/
void OS_Write(FIFO f, int val)
{
	//Handle for the requested FIFO
	FIFOstruct *temp;
	temp->descriptor = NULL;
	temp->indexR = NULL;
	temp->indexW = NULL;
	temp->data = NULL;
	

	//Find the FIFO requested.
	int i;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		temp = masterFIFOArray[i];
		if(temp->descriptor == f)
			break; //break the loop
			//i = 90; //should not get here
	}

	if(temp == NULL)
		return -1;//could not find FIFO requested.
		//FIFO is full, dropping this unread FIFO.

	//We use Modular division to create a circular bounded buffer.
	//When index == FIFOSIZE and you modularly devide it, you get 0. 
	//Meaning, when our index points to the end of the array, once it is modularly 
	//devided, it will point back to the very first element 0, else,
	//any value below the max will remain the same.
	int indexMod = temp->indexW % FIFOSIZE;
	temp->data[indexMod] = val;

	if(temp->indexW == FIFOSIZE)
		temp->indexW = 1;
	else
		temp->indexW += 1;

	return;
}



BOOL OS_Read(FIFO f, int *val)
{
	//Handle for the requested FIFO
	FIFOstruct temp;
	temp.descriptor = NULL;
	temp.indexR = NULL;
	temp.indexW = NULL;
	temp.data = NULL;

	//Find the FIFO requested.
	int i;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		temp = masterFIFOArray[i];
		if(temp.descriptor == f)
			break;
			//i = 90;//break the loop;
	}

	if(temp == NULL)
		return -1;//could not find FIFO requested.

	//We implement the same modular trick for the index
	//as we did in the writer segment.
	int indexMod = temp.indexR % FIFOSIZE;
	
	if(temp.indexR == FIFOSIZE)
		temp.indexR = 1;
	else
		temp.indexR += 1;
	
	//Check to see if FIFO empty
	int check = 0;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		if(temp.data[i] != NULL)
			check = 1;
	}

	if(check != 0)
	{
		//Set the data *val points too with what is in the buffer.
		*val = temp.data[indexMod];
		return TRUE;
	}
	else
		return FALSE;
}


//================================================
//				 START INTERRUPTS
//================================================