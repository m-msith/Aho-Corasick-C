#ifndef _TRIE_
#define _TRIE_

/* 
* AC_Trie.h
*
* Holds the Global Trie structure and related info type that is used throught the program
*
* Globals is a structure that contains a pointer to the current (Cur) and root (Root) nodes of
* the Aho-Corasick pattern trie.
*
* IDCount is a global variable used to keep track and set a unique ID to each State node in the trie
*/
#include "State.h"

struct Globals{

	struct State *Cur;
	struct State *Root;

	int IDCount;

	unsigned long NumPats;
	
};

/* Function that builds an AC pattern trie */
char BuildACTrie(char **patts, struct Globals *g);

/* take care of building the root data structure and initial outputs*/
char BuildACTrieStructure(char **patts, struct Globals *g);

/* take care of adding failure traces as well as tying up output*/
char BuildACTrieFailTraces(struct Globals *g);

/* Function that allows for moving along the Trie structure */
char ACgoto(struct Globals *g, char nxt);

/* Function that returns the current State's fail state */
struct State *GetFailState(struct State *st);

/* Function that processes an input with a particular AC pattern trie */
unsigned int AC_Process(struct Globals *g, char *testString);

/* Function that cleans a global structure, including the ACTrie */
char FreeACTrie(struct Globals *g);

#endif
