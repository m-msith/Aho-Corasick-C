/**
*Contains various tests to run
*/


#include "tests.h"

void testPatStore(char **pats){

  char **hold = pats;
  
  do{
	printf("Found string at level %c, memaddr: %p : %s\n",**pats, pats, *pats);
  }while(**++pats != '\0');
  
  //reset pointer to beginning
  pats = hold;
}

