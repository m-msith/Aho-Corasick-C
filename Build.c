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
	  
	  //add new element to the tree at the appropriate position

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

	  #ifdef PRINT	  
		printf("%c", g->Cur->stc);
	  #endif

	  col++;
	}

	//reset to root for next pattern or task
    //during build, this reset only takes place after the ingestion
    //of one pattern to properly build upon itself with the next one
    //which will either share a branch or build a new one
	g->Cur = g->Root;
	
	#ifdef PRINT
	  printf(" :: inserted pattern num %d into the trie\n", row);	
	#endif
  }

  #ifdef PRINT
	printf("\n~~~Goto Structure Build Completed~~~\n\n");
  #endif

  //add the failure traces in

  //create a first in first out state queue for failure additions
  FifoSteQ *faQ = malloc(sizeof(FifoSteQ *));

  //first add first child state of the trie
  State *qAddSt = g->Root->cState;
  qAddSt->fState = g->Root;
  InitFifoSteQ(faQ, qAddSt);

  //set the initial failure state of level one nodes to the root, null node.
  //add them to the failure addition queue to be considered as failure states for the next level input
  while(qAddSt->sState != NULL){
	#ifdef PRINT
	  printf("Failure Q: Added state %c to the queue\n", qAddSt->stc);
	#endif

	qAddSt = qAddSt->sState;
	
    //1st level always fails to root node
	qAddSt->fState = g->Root;
	PushFifoSteQ(faQ, qAddSt);

  }
  #ifdef PRINT
	printf("Failure Q: Added state %c to the queue\n", qAddSt->stc);
  #endif
 
  //go through queued states until there are no more, signifying that we got through the whole trie 
  while(faQ->head != NULL){

	//pop the queue
	State *popSt = PopFifoSteQ(faQ);	
//	printf("popping this one: %c\n", popSt->stc);

	//visit each of the possible next states of the popped state		  	 
	State *gtSt = popSt->cState;

	while(gtSt != NULL){
	
	  //add the next level states to the fifo queue
	  //this will get us through the entire queue
      //as we cycle until empty
	  PushFifoSteQ(faQ, gtSt);
      
	  #ifdef PRINT
//		printf("Failure Q: Added state %c to the queue\n", gtSt->stc);	  
	  #endif
	 
	  //get and goto every failstate to see if it is a failstate
	  //for the node being tested for failstates
	  State *fSt = GetFailState(popSt); 	    
	  g->Cur = fSt;
      
      //this loop backtraces through the data structure to find the closest (level)
      //failstate. The failstate is a point where the search could continue
      //on a different branch, if the current branch is deemed not tennable.
      //In other words, the failstate fails to the nearest possibility of 
      //a match, whether it start over or continue on a different branch
	  while(ACgoto(g,gtSt->stc) == FALSE){
        
        //break if we're the root node, it's the last possible fail point 
	  	if(g->Cur->stc == '\0'){
		  break;
		}
        else{
        //otherwise, continue to backtrack until we succeed in moving foward
          fSt = GetFailState(fSt);
          g->Cur = fSt;
        }
	  }
	
      //whether it be the root node or a different branch, we have found our 
      //next possible tennable branch, set the fail to that point
	  gtSt->fState = g->Cur;
	  
	  printf(" \"%c\" set to fail at \"%c\"\n", gtSt->stc, gtSt->fState->stc);	
   
      //we process through the trie level by level, so, move through siblings
      //and after processing the last sibling, move to the next child in the 
      //fifo queue
	  gtSt = gtSt->sState;
	}

	
  } 
  
  
  return retVal;

}

