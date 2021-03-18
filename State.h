#ifndef _STATE_
#define _STATE_

/**
* Contains references to needed libraries by State.c as well as function prototypes and variables 
*
* State.h - by Martin Smith
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

	/* output is variable in size, uses a linked list of characters */
	struct Output *ot_head;

};

/*
* Init a default state with ID, character, and null pointer values
*/
void DefaultStateInit(char c, unsigned long id, struct State *st);

/* 
* Add a new Output structure to a given Output linked list 
*/
unsigned char AppendOutput(char c, struct Output **o);

/*
* Concatanate two Output structure linked lists
*/
unsigned char CatOutput(struct Output *from, struct Output **to);

/*
* Clean up an Output structure linked list 
*/
unsigned char FreeOutput(struct Output *o);

#endif
