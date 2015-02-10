#define MAXFIFO            16     /* max. # of FIFOs supported */
#define FIFOSIZE           8      /* max. # of data elements per FIFO */

typedef unsigned int BOOL;
typedef unsigned int FIFO;

//================================================
//				 STOP INTERRUPTS
//================================================
//=================
//Structs

typedef struct FIFO_t
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
	newFIFO->descriptor = newFIFO->data;
	newFIFO->indexW = 0;
	newFIFO->indexR = 0;

	//initialize every element in the buffer to NULL
	int i;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		newFIFO->data[i] = NULL;
	}

	//Parse the master array, find a spot for new FIFO
	for(i = 0; i < MAXFIFO -1; i++)
	{
		if(masterFIFOArray[i] == NULL)
		{
			masterFIFOArray[i] = newFIFO->descriptor;
			newFIFO->index = i;
			return newFIFO->descriptor;
		}
	}

	//if we make it here, it means we have our limit of FIFO
	//return error.
	return -1;
}



void OS_Write(FIFO f, unsigned int val)
{
	//Handle for the requested FIFO
	FIFOstruct temp = NULL;

	//Find the FIFO requested.
	int i;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		temp = masterFIFOArray[i];
		if(temp->descriptor == f)
			i = 90;//break the loop;
	}

	if(temp == NULL)
		return -1;//could not find FIFO requested.

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



BOOL OS_Read(FIFO f, unsigned int *val)
{
	//Handle for the requested FIFO
	FIFOstruct temp = NULL;

	//Find the FIFO requested.
	int i;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		temp = masterFIFOArray[i];
		if(temp->descriptor == f)
			i = 90;//break the loop;
	}

	if(temp == NULL)
		return -1;//could not find FIFO requested.

	//We implement the same modular trick for the index
	//as we did in the writer segment.
	int indexMod = temp->indexR % FIFOSIZE;
	
	if(temp->indexR == FIFOSIZE)
		temp->indexR = 1;
	else
		temp->indexR += 1;
	
	//Check to see if FIFO empty
	int check = 0;
	for(i = 0; i < MAXFIFO -1; i++)
	{
		if(temp->data[i] != NULL)
			check = 1;
	}

	if(check != 0)
	{
		//Set the data *val points too with what is in the buffer.
		*val = temp->data[indexMod];
		return TRUE;
	}
	else
		return FALSE;
}


//================================================
//				 START INTERRUPTS
//================================================