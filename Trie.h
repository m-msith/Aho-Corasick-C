#ifndef _TRIE_
#define _TRIE_

/* 
* Trie.h
*
* Holds the Global Trie structure and related info type that is used throught the program
*
* Globals is a structure that contains a pointer to the current (Cur) and root (Root) nodes of
* the Aho-Corasick pattern trie.
*
* IDCount is a global variable used to keep track and set a unique ID to each State node in the trie
*/
#include "State.h"

typedef struct Globals{

	State *Cur;
	State *Root;

	int IDCount;

	unsigned long NumPats;
	
}Globals;


#endif
