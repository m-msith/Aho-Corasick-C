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

	/* output is variable in size, uses a linked list of characters */\
	struct Output *ot_head;

};

/*
* Init a default state with ID, character, and null pointer values
*/
void DefaultStateInit(char c, int id, struct State *st);

/* 
* Add a new Output structure to a given Output linked list 
*/
char AppendOutput(char c, struct Output **o);

/*
* Concatanate two Output structure linked lists
*/
char CatOutput(struct Output *from, struct Output **to);

/*
* Clean up an Output structure linked list 
*/
char FreeOutput(struct Output *o);

#endif
