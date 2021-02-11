/*
* SteQ.c - By: Martin Smith
*
* Contains functions used to initialize and manage a first in first out queue of states
*
* Used by Build.c
*/

#include "SteQ.h"

/*
* Default initializaiton for a state queue
*/
void InitSteQ(struct SteQ *stq, struct State *s){

	stq->stp = s;
	stq->nxt = NULL; 

}

/**
* Default initialization for a first in first out state queue
*/
char InitFifoSteQ(struct FifoSteQ *fq, struct State *s){

    char ret = TRUE;

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
	fq->end = fq->head;

    return ret;
}

/*
* Add a member to a fifo state queue end
*/
void PushFifoSteQ(struct FifoSteQ *fq, struct State *s){

	fq->end->nxt = malloc(sizeof(struct SteQ));

	if(fq->end->nxt == NULL){
		printf("FAILED MALLOC STATE QUEUE PUSH\n");
	}

	fq->end = fq->end->nxt;

	InitSteQ(fq->end, s);

	//todo
	/* band-aid for the moment, may be the right fix though, need to investigate */
	if(fq->head == NULL){
		fq->head = fq->end;
	}
  
}

/*
* Remove a member from the queue and return a pointer to the member that was removed
* If the head is at NULL, there are no states in the queue, return NULL
*/
struct State *PopFifoSteQ(struct FifoSteQ *fq){

	struct State *retSt;

	if(fq->head != NULL){

		retSt = fq->head->stp;

		/* change head position to the next state */
		struct SteQ* rem = fq->head;
		fq->head = fq->head->nxt;

		/* don't kill the state in the process of removal/freeing of the head queue member */
		rem->stp = NULL;
		free(rem);
		
	}
	else{

		retSt = NULL;

	}

	return retSt;

}


