#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>


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
int meow = 420;
sem_t sem_name;

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
  /* If node to be deleted is head node */
  /* Change next only if node to be deleted is NOT the last node */
  /* Change prev only if node to be deleted is NOT the first node */
  /* Finally, free the memory occupied by del*/

/**
  if(*head_ref == NULL || del == NULL)
    return;

  if(*head_ref == del)
    *head_ref = del->next;

  if(del->next != NULL)
    del->next->prev = del->prev;

  if(del->prev != NULL)
    del->prev->next = del->next;


  return;

**/
}










//==================
//Semaphore Functions

//Here we lay out memory for our new node.
void OS_InitSem(int sem_ID, int max_num)
{
	//Lay out memory for our semaphore
	semaphore *sem = malloc(1024); //<-- DONT FUCKING MALLOC

	//Initialize some sem data
	sem->semID = sem_ID;
	sem->lock = 1;
	//sem->prev = NULL;
	sem->next = NULL;

	//Throw our new semaphore in front of the list of sems.
	enqueueSemaphore(&masterSem, sem);
}



void OS_Wait(int sem_ID)
{
	sem_wait(&sem_name);

	//Create a handler for the requested sem
	semaphore *tmp;
	tmp = findSemID(masterSem, sem_ID);

	/**
	 * PCB's should have their own little lock to indicate
	 * that they are the one who can go next. like the while
	 * busy loop below is still there but it would wait on the
	 * value for that individual PCB. Then pick the PCB in the
	 * list that is up for the CS and change its lock value.
	 */
	if(tmp->lock <= 0)
	{
		sem_post(&sem_name);
		//decrement lock
		//tmp->lock -= 1;



		//Check if lock is in use
		while(tmp->lock <= 0){
			//Add process to back of list.
			sleep(3);//busy wait
		}
	}
	else // lock was a 1 so no list
	{
		sem_post(&sem_name);
		//decrement lock
		tmp->lock -= 1;
	}

	printf("The lock value: %d\n", tmp->lock);

}




void OS_Signal(int sem_ID)
{
	sem_wait(&sem_name);

	//Create a handler for the requested sem
	semaphore *sem;
	sem = findSemID(masterSem, sem_ID);

	//increment lock
	sem->lock += 1;

	//Remove the process from a list


	sem_post(&sem_name);
	return;
}


int test(int ID)
{
	printf("Thread #%d calling sem\n", ID);
	OS_Wait(69);

	meow -= 1;
	puts("im all up in it dude");

	int x = 0;
	while(x < 10)
	{
		sleep(1);
		x++;
	}

	OS_Signal(69);

	return 0;
}


void main()
{
	pthread_t	*threads[3];

	OS_InitSem(69, 1);

	sem_init(&sem_name, 0, 1);
	puts("we have inited");

	int i = 0;
	for (i = 0; i < 3; i++)
		threads[i] = (pthread_t *)malloc(sizeof(pthread_t));
	for (i = 0; i < 3; i++)
		pthread_create(&(threads[i]), NULL, &test, i);
	for (i = 0; i < 3; i++)
		pthread_join(threads[i], NULL);

	return;

}
