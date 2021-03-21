/*
* Contains functions used to initialize and manage a first in first out queue of states
*
* SteQ.c - By: Martin Smith
*/

#include "SteQ.h"

/*
* Default initializaiton for a state queue
*
* params-
*
* stq (struct SteQ*): the state queue to init 
* s (struct State*): the state to add to stq
*/
void InitSteQ(struct SteQ *stq, struct State *s){

	stq->stp = s;
	stq->nxt = NULL; 

}

/**
* Default initialization for a first in first out state queue
*
* return-
*
* (unsigned char): pass or fail 
*
* params-
*
* fq (struct FifoSteQ*): The Fifo State Queue to Init
* s (struct State*): the state to init the state queue with
*
*/
unsigned char InitFifoSteQ(struct FifoSteQ *fq, struct State *s){

    unsigned char ret = TRUE;

	fq->head = malloc(sizeof(struct SteQ));
    /* Allocation check */
    if(fq->head == NULL){
        return FALSE;
    }
    
	fq->end = malloc(sizeof(struct SteQ));
    /* Allocation check */
    if(fq->end == NULL){
        return FALSE;
    }

	InitSteQ(fq->head, s);
	InitSteQ(fq->end, NULL);
	fq->head->nxt = fq->end;

    return ret;
}

/*
* Add a member to a fifo state queue end, or init if empty 
*
* params-
*
* fq (struct FifoSteQ *): the queue to push
* s (struct State *): the state to push on the queue 
*
*/
void PushFifoSteQ(struct FifoSteQ *fq, struct State *s){

	if(fq->end->stp == NULL){	
	
		fq->end->stp = s;
		fq->end->nxt = malloc(sizeof(struct SteQ));
		if(fq->end->nxt == NULL){
			printf("FAILED MALLOC STATE QUEUE PUSH\n");
		}
		
		fq->end = fq->end->nxt;
		fq->end->stp = NULL;
		
	}

}

/*
* Remove a member from the queue and return a pointer to the member that was removed
* If the head is at NULL, there are no states in the queue, return NULL
*
* return-
*
* (struct State *): the state that was just popped off of the queue 
*
* params-
*
* fq (struct FifoSteQ *): the queue that we want to pop
*
*/
struct State *PopFifoSteQ(struct FifoSteQ *fq){

	struct State *retSt;

	if(fq->head != NULL){

		retSt = fq->head->stp;
		
		if(fq->head->nxt != NULL){
			/* change head position to the next state */
			struct SteQ* rem = fq->head;
			fq->head = fq->head->nxt;

			/* don't kill the state in the process of removal/freeing of the head queue member */
			rem->stp = NULL;
			free(rem);
			rem = NULL;
		}
	}
	else{

		retSt = NULL;

	}

	return retSt;

}


