#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



//=================
//Structs

typedef struct sem_t
{
	//Used to ID the sem
	int semID;

	//The position of this semaphore in the
	//master active semaphore array.
	int index;

	//The actual mutex value.
	int lock;

	//For semaphore queue
	struct sem_t* next;

	//Dead process list stuff
	struct Node* nextPCB;
	struct Node* prevPCB;
}semaphore;







//=================
//Globals

//Used as a handler for our queue of sems.
semaphore* masterSem = NULL;







//==================
//Queue Functions

/*
 * The semaphores work as a giant queue. The masterSem is the master head of the semaphore queue.
 * Each semaphore that is created gets appended to the cue that masterSem is the head of.
 */
int enqueueSemaphore(semaphore **head, semaphore *newSem)
{
    semaphore *tmp;

    if (newSem == NULL)
        return -1;

    //Case where it is the first one in the list.
    if (*head == NULL)
    {
        *head = newSem;
        newSem->next = NULL;
    }
    //Else the list is not empty, append this sem to the tail.
    else
    {
        tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = newSem;
        newSem->next = NULL;
    }
    return 1;
}



semaphore* dequeue(semaphore **head)
{
    semaphore *tmp = NULL;

    if (*head != NULL)
    {
        tmp = *head;
        *head = (*head)->next;
    }
    return tmp;
}



//Given a semaphore queue, it will find the matching semaphore
//for a given sem_ID
semaphore* findSemID(semaphore *head, int sem_ID)
{
    semaphore *tmp = head;
    while (tmp != NULL)
        if (tmp->semID == sem_ID)
            return tmp;
        else
            tmp = tmp->next;
    return tmp;
}



void deleteNode(semaphore **head_ref, semaphore *del)
{
  /* base case */
  if(*head_ref == NULL || del == NULL)
    return;

  /* If node to be deleted is head node */
  if(*head_ref == del)
    *head_ref = del->next;

  /* Change next only if node to be deleted is NOT the last node */
  if(del->next != NULL)
    del->next->prev = del->prev;

  /* Change prev only if node to be deleted is NOT the first node */
  if(del->prev != NULL)
    del->prev->next = del->next;

  /* Finally, free the memory occupied by del*/
 // free(del);
  return;
}












//==================
//Semaphore Functions

//Here we lay out memory for our new node.
void OS_InitSem(int sem_ID, int max_num)
{
	//Lay out memory for our semaphore
	struct semaphore sem = (struct semaphore*)malloc(sizeof(int)); //<-- DONT FUCKING MALLOC

	//Initialize some sem data
	sem->semID = sem_ID;
	sem->lock = 0;
	sem->prev = NULL;
	sem->next = NULL;

	//Throw our new semaphore in front of the list of sems.
	enqueueSemaphore(&masterSem, sem);
}



void OS_Wait(int sem_ID)
{
	//Create a handler for the requested sem
	semaphore *tmp;
	tmp = findSemID(masterSem, sem_ID);

	//decrement lcok
	tmp->lock -= 1;

	//Check if lock is in use
	while(tmp->lock < 0); //<- busy wait for lock

	//Add the process to a list????
}




void OS_Signal(int sem_ID)
{
	//Create a handler for the requested sem
	semaphore *sem;
	sem = findSemID(masterSem, sem_ID);

	//increment lcok
	sem->lock += 1;

	if(sem->lock <= 0)
		return;
	//Remove the process from a list????
}








