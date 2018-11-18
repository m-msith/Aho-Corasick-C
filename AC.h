/*
* AC.h - By: Martin Smith
*
* Include needed libraries and prototypes for the AC.c file.
*
* Used by AC.c
*/

#include "State.h"
#include "IO.h"
#include "tests.h"

/*
*Globals is a structure that contains a pointer to the current (Cur) and root (Root) nodes of
* the Aho-Corasick pattern trie.
*
* IDCount is a global variable used to keep track and set a unique ID to each State node in the trie
*/
typedef struct Globals{

State *Cur;
State *Root;

int IDCount;
}Globals;

//Method used for initalizing the values of the the globals structure
Globals* InitGlobals();

//Method use for the cleanup of the globals structure
void CleanGlobals(Globals *g);
