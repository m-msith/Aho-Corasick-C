#ifndef _BUILD_
#define _BUILD_

/*
* Build.h - By: Martin Smith
*
* Contains libraries needed by Build.c as well as function prototypes
*
* Used by AC.c/h 
*/

#include "Trie.h"
#include "SteQ.h"

char BuildACTrie(char **patts, Globals *g);

#endif