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
*SteQ is a simple linked list of states that is used as a queue in order to build in the failure traces
* into the AC pattern trie. 
*/
typedef struct SteQ{

State *stp;
struct SteQ *nxt;

}SteQ;

#endif
