#include "FIFO.h"
#include "nios2_ctrl_reg_macros.h"
#define NULL 0

fifoCount = 100;

struct fifoStruct fifoArray[MAXFIFO];

FIFO OS_InitFiFo()
{
	NIOS2_WRITE_STATUS(0);

	//Create some elements to build the new FIFO
	struct fifoStruct newFIFO;

	//Creates an ID for the FIFO
	newFIFO.descriptor = fifoCount;
	fifoCount++;
	
	//To start the reader and writer at 0
	newFIFO.indexW = 0;
	newFIFO.indexR = 0;

	//initialize every element in the buffer to NULL
	int i;
	for(i=0; i < MAXFIFO; i++)
	{
		if(fifoArray[i].used == 0)
			break; 
	}

	fifoArray[i] = newFIFO;
	//printf("VALUE OF I: %d\n", i);
	return newFIFO.descriptor;

	NIOS2_WRITE_STATUS(1);
}




//===========================
//		FIFO FUNCTIONS
//===========================
void OS_Write(FIFO f, int val)
{
	NIOS2_WRITE_STATUS(0);

	//Find the FIFO requested.
	int i;
	for(i = 0; i < MAXFIFO; i++)
	{
		if(fifoArray[i].descriptor == f)
			break; 
	}

	//We use Modular division to create a circular bounded buffer.
	int indexMod = fifoArray[i].indexW % FIFOSIZE;
	fifoArray[i].data[indexMod] = val;
	//printf("VALUE Written: [%d][%d]\n", fifoArray[i].data[indexMod], i);


	//Here, to keep our cicle working, we have to reset the index
	//if it has hit the limit.
	if(fifoArray[i].indexW == FIFOSIZE)
		fifoArray[i].indexW = 1;
	else
		fifoArray[i].indexW += 1;

	NIOS2_WRITE_STATUS(1);
}




BOOL OS_Read(FIFO f, int *val)
{
	NIOS2_WRITE_STATUS(0);

	//Find the FIFO requested.
	int i;
	for(i = 0; i < MAXFIFO; i++)
	{
		if(fifoArray[i].descriptor == f)
			break; 
	}

	//Set the reader position in the buffer
	int indexMod = fifoArray[i].indexR % FIFOSIZE;
	
	if(fifoArray[i].indexR == FIFOSIZE)
		fifoArray[i].indexR = 1;
	else
		fifoArray[i].indexR += 1;
	
	//Check to see if FIFO empty
	int check = 0;
	int j;
	for(j = 0; j < MAXFIFO; j++)
	{
		//If ANY member is not empty, don't read
		if(fifoArray[j].data[j] != NULL)
			check = 1;
	}

	if(check == 1)
	{
		//Set the data *val points too with what is in the buffer.
		*val = fifoArray[i].data[indexMod];
		//printf("VALUE Read: [%d][%d][%d]\n", fifoArray[i].data[indexMod], i, fifoArray[i].indexW);
		//printf("VAL: [%d]", *val);
		return TRUE;
	}
	else
		return FALSE;

	NIOS2_WRITE_STATUS(1);
}



//===========================
//		END FIFO FUNCTIONS
//===========================

