#ifndef _STEQ_
#define _STEQ_

/*
* Contains libraries needed by SteQ.c as well as function prototypes from SteQ.c
*
* SteQ.h - By: Martin Smith
*/
#include "State.h"

/*
* SteQ is a simple linked list of states that is used for a fifo queue in order to build in the failure traces
* into the AC pattern trie. 
*/
struct SteQ{

	struct State *stp;
	struct SteQ *nxt;

};

/*
* FifoSteQ is a one stop container to hold a first in first out queue of states.
*
* Init a FifoSteQ by calling InitFifoSteQ()
* Push a FifoSteQ by calling PushFifoSteQ()
* Pop a FifoSteQ by calling PopFifoSteQ()
* 
*/
struct FifoSteQ{

	struct SteQ *head;
	struct SteQ *end;

};

/*
*Takes care of initializing a SteQ struct with a state and a Null pointer for its next element
*/
void InitSteQ(struct SteQ *stq, struct State *s);


/*
*Takes care of initializing a FifoSteQ struct with a head and end SteQ's
*/
unsigned char InitFifoSteQ(struct FifoSteQ *fq, struct State *s);

/*
*Add a state to a FifoSteQ (changes the end SteQ)
*/
void PushFifoSteQ(struct FifoSteQ *fq, struct State *s);

/*
*Remove a state from the FifoSteQ (sets the head to the next member and removes the previous head)
*
*Returns the state that was removed from the fifo queue
*/
struct State *PopFifoSteQ(struct FifoSteQ *fq);

/*
* Free a queue's members
*/
void CleanFifoSteQ(struct FifoSteQ *fq);

#endif
