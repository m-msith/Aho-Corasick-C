#ifndef _AC_
#define _AC_

/*
* AC.h - By: Martin Smith
*
* Include needed libraries and prototypes for the AC.c file.
*
* Used by AC.c
*/

#include "State.h"
#include "Trie.h"
#include "IO.h"
#include "tests.h"
#include "Build.h"

//Method used for initalizing the values of the the globals structure
Globals* InitGlobals();

//Method use for the cleanup of the globals structure
void CleanGlobals(Globals *g);

//Function that allows for moving along the Trie structure
char ACgoto(Globals *g, char nxt);

//Function that returns the current State's fail state
State *GetFailState(State *st);
#endif
