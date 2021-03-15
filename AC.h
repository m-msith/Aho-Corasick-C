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
void InitGlobals(struct Globals *g);

/* Method use for the cleanup of the globals structure */
char CleanGlobals(struct Globals *g);

#endif
