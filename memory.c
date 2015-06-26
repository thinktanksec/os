#include "memory.h"
#include "nios2_ctrl_reg_macros.h"
#include "process.h"
#define NULL 0


processCount = 0;
memStruct arrayOfBlocks[30];

//===========================
//        QUEUE CALLS
//===========================


int enqueueBlock(memStruct **head, memStruct *newBlock)
{
    memStruct *tmp;

    if (newBlock == NULL)
        return -1;

    //Case where it is the first one in the list.
    if (*head == NULL)
    {
        *head = newBlock;
        newBlock->next = NULL;
    }
    //Else the list is not empty, append this sem to the tail.
    else
    {
        tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = newBlock;
        newBlock->next = NULL;
    }
    return 1;
}




memStruct* dequeueBlock(memStruct **head)
{
    memStruct *tmp = NULL;

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
memStruct* findBlock(memStruct *head, MEMORY add)
{
    memStruct *tmp = head;

    while (tmp != NULL)
    {
        if (tmp->address == add)
            return tmp;
        else
            tmp = tmp->next;
    }

    return 0;
}
//===================================
//          END QUEUE CALLS
//===================================







//            _.-.
//          ,'/ //\
//         /// // /)
//        /// // //|
//       /// // ///
//      /// // ///
//     (`: // ///
//      `;`: ///
//      / /:`:/
//     / /  `'
//    / /
//   (_/








//==================================
//          MEMORY CALLS
//==================================

void   OS_InitMemory()
{
    int i;
    //Set the head of queue to NULL
    memoryQueueHead = NULL;

    //Init evey member to zero
    for(i = 0; i < 4096; i++)
        mainMemory[i] = 0;

    //Make every struct, memory container, available
    memStruct *tmp;
    for(i = 0; i < 30; i++)
        arrayOfBlocks[i].available = 1;
}



MEMORY OS_Malloc(int val)
{
    NIOS2_WRITE_STATUS(0);
    //Grab an available memory container block.
    int i;
    for(i = 0; i < 30; i++)
    {
        if(arrayOfBlocks[i].available == 1)
        {
            arrayOfBlocks[i].available = 0;
            i = 90;
        }
    }

    //Assign this block to the calling PCB
    //process *tmp = currentProc; //<-- Change this to some global that is set with each process change.
   // tmp->blockIndex = i;


    //Search for an empty hole in memory
    //Using a type of first fit.
    int holeCount;
    MEMORY add = -1;
    for(i = 0; i < 4096; i++)
    {
        if(mainMemory[i] == 0)
        {
            int j;
            holeCount = 1;
            for(j = i+1; j < 4096; j++)
            {
                if(holeCount == val)
                {
                    arrayOfBlocks[i].address = &mainMemory[i];
                    arrayOfBlocks[i].length = val;
                    arrayOfBlocks[i].index = i;
                    add = i; //<-- Keep a copy of the index.
                    i = 9999; //<-- break out of the other for loop.
                    break;
                }

                if(mainMemory[j] != 0)
                    break;
                else
                    holeCount++;
            }
        }
    }


    //set members in that hole != 0 (not available)
    //they are now ready to use.
    //if add is still 0, we couldn't service request.
    if(add != -1)
    {
        int count = 0;
        i = add;
        while(count < val)
        {
            mainMemory[add] = 1;
            i++;
            count++;
        }
    }

    NIOS2_WRITE_STATUS(1);
    //return memory ID for this struct.//
    return arrayOfBlocks[add].address;
}




BOOL OS_Free(MEMORY m)
{
    //Locate that entry
    memStruct *block;

    //Find the block being requested.
    int i;
    int found = 0;
    for(i = 0; i < 30; i++)
    {
        if(arrayOfBlocks[i].address == m)
        {
            found = 1;
            break;
        }
    }

    //If block could not be found.
    if(block == 0)
        return FALSE;
    else
    {
        //Set the memory holes to available
        int index;
        for(index = arrayOfBlocks[i].index; index <= arrayOfBlocks[i].length; index++)
            mainMemory[index] = 0;

        //Free the block struct and update the process.
        arrayOfBlocks[i].available = 1;
       // currentProc->blockIndex = NULL;

        return TRUE;
    }
}
//====================================
//          END MEMORY CALLS
//====================================


