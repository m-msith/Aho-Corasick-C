#ifndef _STEQ_
#define _STEQ_

/*
* SteQ.h - By: Martin Smith
*
* Contains libraries needed by SteQ.c as well as function prototypes from SteQ.c
*
* Used by Build.c/h
*/

#include "State.h"

/*
* SteQ is a simple linked list of states that is used for a fifo queue in order to build in the failure traces
* into the AC pattern trie. 
*/
typedef struct SteQ{

  State *stp;
  struct SteQ *nxt;

}SteQ;

/*
* FifoSteQ is a one stop container to hold a first in first out queue of states.
*
* Init a FifoSteQ by calling InitFifoSteQ()
* Push a FifoSteQ by calling PushFifoSteQ()
* Pop a FifoSteQ by calling PopFifoSteQ()
* 
*/
typedef struct FifoSteQ{

  SteQ *head;
  SteQ *end;

}FifoSteQ;

/*
*Takes care of initializing a SteQ struct with a state and a Null pointer for its next element
*/
void InitSteQ(SteQ *stq, State *s);


/*
*Takes care of initializing a FifoSteQ struct with a head and end SteQ's
*/
void InitFifoSteQ(FifoSteQ *fq, State *s);

/*
*Add a state to a FifoSteQ (changes the end SteQ)
*/
void PushFifoSteQ(FifoSteQ *fq, State *s);

/*
*Remove a state from the FifoSteQ (sets the head to the next member and removes the previous head)
*
*Returns the state that was removed from the fifo queue
*/
State *PopFifoSteQ(FifoSteQ *fq);


#endif
