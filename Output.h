#ifndef _OUTPUT_
#define _OUTPUT_

/**
* Output.h - Martin Smith
*
* Contains Libraries required by the Output.c and function prototypes.
* 
* Used by the State.h header file
*/

#include "DefLib.h"

/**
* The Output structure is a linked list of characters, using double pipe as deliminating char
*/
typedef struct Output{

char c;
struct Output *nxt;

}Output;

#endif
