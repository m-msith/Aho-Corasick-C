#ifndef _AC_
#define _AC_

/*
* AC.h - By: Martin Smith
*
* Include needed libraries and prototypes for the AC.c file.
*
* Used by AC.c
*/

#include "IO.h"
#include "tests.h"
#include "AC_Trie.h"

/* Method used for initalizing the values of the the globals structure */
struct Globals *InitGlobals();

/* Method use for the cleanup of the globals structure */
void CleanGlobals(struct Globals *g);

#endif
