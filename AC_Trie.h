#ifndef _TRIE_
#define _TRIE_

/* 
* Holds the Trie structure and related info type that is used throught the program
*
* AC_Trie is a structure that contains a pointer to the current (Cur) and root (Root) nodes of
* a Aho-Corasick pattern trie.
*
* IDCount is a variable used to keep track and set a unique ID to each State node in the trie
*
* AC_Trie.h - By: Martin Smith
*/
#include "State.h"

struct AC_Trie{

	struct State *Cur;
	struct State *Root;

	int IDCount;

	unsigned long NumPats;
	
};

/* Function that builds an AC pattern trie */
unsigned char BuildACTrie(char **patts, struct AC_Trie *act);

/* Method used for initalizing the values of the the AC_Trie structure */
unsigned char InitACTrie(struct AC_Trie *act);

/* take care of building the root data structure and initial outputs*/
unsigned char BuildACTrieStructure(char **patts, struct AC_Trie *act);

/* take care of adding failure traces as well as tying up output*/
unsigned char BuildACTrieFailTraces(struct AC_Trie *act);

/* Function that allows for moving along the Trie structure */
unsigned  char ACgoto(struct AC_Trie *act, char nxt);

/* Function that returns the current State's fail state */
struct State *GetFailState(struct State *st);

/* Function that processes an input with a particular AC pattern trie */
unsigned long AC_Process(struct AC_Trie *act, char *testString);

/* Method use for the cleanup of the AC_Trie container structure */
unsigned char CleanACTrie(struct AC_Trie *act);

/* Function that cleans a AC_Trie structure */
unsigned char FreeACTrie(struct AC_Trie *act);


#endif
