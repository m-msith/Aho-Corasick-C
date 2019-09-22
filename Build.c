/**
* Build.c
*
* This file is used to take care of building the AC Trie. The AC trie consists of three build steps: Building the 
* Trie structure, building the Fail traces into the structure, and building the output structure into each node
* of the Trie structure.
*
*/

#include "Build.h"
#include "AC.h"

//Paren Build function
//Returns: 1 if fail during building Trie structure, 2 if during building fail traces, 3 if during building output
int BuildACTrie(char **patts, Globals *g){

  int retVal = 0;
  //ACgoto(g, nxt)
  int row = 0, useCst = TRUE;
  
  for(row = 0; row < g->NumPats; row++){

	//reset counting var
	int col = 0;
	
	//add a string at a time
	while(patts[row][col] != '\0'){
	  //only ever one child element, else have to use the child's sibling element
	  if(useCst == TRUE){
	  	g->Cur->cState = malloc(sizeof(State));

		if(g->Cur->cState == NULL){
		  //failed allocation
		  printf("ERROR Build #1\n");
	  	  return 1;		  
		}
				
		g->Cur = g->Cur->cState;		
	  }
	  
	  //defaultState init
	  DefaultStateInit(patts[row][col], g->IDCount++, g->Cur);

	  col++;
	}

	
  }
  return retVal;
}

