#ifndef _STATE_
#define _STATE_

/**
* State.h - by Martin Smith
*
* Contains references to needed libraries by State.c as well as function prototypes
*
* Used by AC.c/h SteQ.c/h 
*/

#include "DefLib.h"

/**
* The Output structure is a linked list of characters, using null as deliminating char
*/
struct Output{

	char c;
	struct Output *nxt;

};

/**
* State the structure for a given point in the AC !state! trie. 
*/
struct State{

	char stc; /* At the simplest level, a state is just a character */
	int id;

	/* Pointers for child, sibling, and fail states respectively */
	struct State *cState;
	struct State *sState;
	struct State *fState;

	/* Output is variable, uses a linked list of characters */
	struct Output *ot;
	/* include ref to head for reset */
	struct Output *ot_head;

};

/*
* Init a default state with ID, character, and null pointer values
*/
void DefaultStateInit(char c, int id, struct State *st);

/* 
* Add a new Output structure to a given one without messing with the 
* given one.
*/
char AppendOutput(char c, struct Output **o);

char CatOutput(struct Output *from, struct Output **to);

#endif
