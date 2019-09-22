/** * Build.c
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
char BuildACTrie(char **patts, Globals *g){

  char retVal = 0;
  int row = 0;
  
  //add a string a char at a time
  for(row = 0; row < (g->NumPats - 1); row++){ //skipping last null entry for now

	//reset counting var
	int col = 0;
			
	while(patts[row][col] != '\0'){

	  //cycle to the first available entry point
	  while(ACgoto(g, patts[row][col]) == TRUE){}	  
	  //only ever one child element, else have to use the child's sibling element
	  if(g->Cur->cState == NULL){
	  	g->Cur->cState = malloc(sizeof(State));

		if(g->Cur->cState == NULL){
		  //failed allocation
		  printf("ERROR Build #1\n");
	  	  return 1;		  
		}
				
		g->Cur = g->Cur->cState;		
	  }
	  //go to child's sibling state if there is a child state already
	  else{

		g->Cur = g->Cur->cState;	  	

		//need to cycle through the sibling states
		while(g->Cur->sState != NULL){
			g->Cur = g->Cur->sState;
		}

	  	g->Cur->sState = malloc(sizeof(State));
		
		if(g->Cur->sState == NULL){
		  //failed allocation
		  printf("ERROR BUILD #2\n");
		  return 1;
		}
	  	
		g->Cur = g->Cur->sState;

	  }
	  
	  //defaultState init
	  DefaultStateInit(patts[row][col], g->IDCount++, g->Cur);
	  
	  printf("%c", g->Cur->stc);

	  col++;
	}

	//reset to root for next pattern or task
	g->Cur = g->Root;
	
	#ifdef PRINT
	  printf(" :: inserted pattern num %d into the trie\n", row);	
	#endif
  }

  return retVal;
}

