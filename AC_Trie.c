/*
* This file is used to take care of building the AC Trie and running the AC algorithm. 
* 
* Building the AC trie consists of three build steps: Building the Trie structure, 
* building the Fail traces into the structure, and building the output structure into each node
* of the Trie structure. Output building is taken care of at the end of building the structure 
* and at the end of building the fail traces. 
*
* Running through the AC trie requires an input string and an AC trie. The processing function
* goes through the input string char by char, and processes the next state of the trie to determine
* possible matches.
*
* AC_Trie.c - By: Martin Smith
*/

#include "AC_Trie.h"
#include "SteQ.h"

/* 
* Trie Build function
* Returns: 
* 1 if fail during structure init
* 2 if fail during structure build
* 3 if fail during failtrace building
*/
unsigned char BuildACTrie(char **patts, struct AC_Trie *act){

	unsigned char retVal = 0;
	
	retVal = InitACTrie(act);
    retVal = BuildACTrieStructure(patts, act);
    retVal = BuildACTrieFailTraces(act);
    
	return retVal;

}

/**
* Take care of initializing the AC_Trie data structure to be used for pattern matching
*
* returns-
*
* (unsigned char): whether or not the function failed
*
* params-
*
* act (AC_Trie *): the AC_Trie to initialize
*
*/
unsigned char InitACTrie(struct AC_Trie *act){	

	unsigned char retVal = FALSE;

	act->Root = malloc(sizeof(struct State));
	if(act->Root == NULL){
		retVal = 1;
	}

	/* init root */
	DefaultStateInit('\0', 0, act->Root);

	/* root node is null and fails to itself */
	act->Root->fState = act->Root;
	act->Cur = act->Root;

	act->IDCount = 0;
	
	return retVal;

}

/* Using the 2D pattern import, build the AC_Trie. The Trie is built by 
* cycling through the patterns char by char and creating a state for the 
* char and placing it on the trie at the correct position. 
* 
* returns-
* (unsigned char): the fail/success of the build
* 
* params-
*
* patts (char **): array of char *, a pattern per char *
* act (AC_Trie*): pointer to the AC_Trie structure
*
* */
unsigned char BuildACTrieStructure(char **patts, struct AC_Trie *act){
    
    unsigned char retVal = FALSE;
    unsigned long row = 0;
  
	#ifdef PRINT
		printf("!!!!BUILDING TRIE!!!!\n\n");
		printf("~~~Building Goto Structure + Output~~~\n\n");
	#endif
  
	/* Add a patterns, char at a time */
	for(row = 0; row < act->NumPats; row++){ 

		unsigned long col = 0;
	
		/* When we hit null, it's time to move to the next string */
		while(patts[row][col] != '\0'){

			/* Cycle through the trie to the first available new state point */
			while(ACgoto(act, patts[row][col]) == TRUE){
		
				#ifdef PRINT	
					printf("skipping %c, already in the trie @ %p\n", patts[row][col], act->Cur);
				#endif
			
				col++;
			}	  
		
			/* Add new element to the tree at the appropriate position if we didn't cycle to the last value */
			if(patts[row][col] != '\0'){
			  
				/* priority is to add a child element */
				if(act->Cur->cState == NULL){
					
					act->Cur->cState = malloc(sizeof(struct State));
					/* failed allocation */
					if(act->Cur->cState == NULL){
						printf("ERROR Build #1\n");
						retVal = 2;		  
					}
						
					act->Cur = act->Cur->cState;		
				}
				/* add a sibling if there is already a child */
				else{
					
					act->Cur = act->Cur->cState;	  	

					/* cycle to the last sibling state to postpend */
					while(act->Cur->sState != NULL){
						act->Cur = act->Cur->sState;
					}

					act->Cur->sState = malloc(sizeof(struct State));
					/* failed allocation */
					if(act->Cur->sState == NULL){
						printf("ERROR BUILD #2\n");
						retVal = 2;
					}

					act->Cur = act->Cur->sState;
				}
			  
				/* Init the new state */
				DefaultStateInit(patts[row][col], act->IDCount++, act->Cur);

				#ifdef PRINT	  
					printf("Adding state \"%c\" @ %p\n", act->Cur->stc, act->Cur);
				#endif


				col++;
			}
			else{
				/* We don't add a new state because the pattern is already in
				*  the trie, need to add the output though */
			}

		}

		/* reset col for output */
		col = 0;

		#ifdef PRINT
			printf("adding output: ");
		#endif

		/* Add the output to the last state of the addition */
		while(patts[row][col] != '\0'){
			
            AppendOutput(patts[row][col], &act->Cur->ot_head);
            
			#ifdef PRINT
				printf("%c", patts[row][col]);
			#endif
			
			col++;
		}
	
		/* set to null to delimit output, also good practice to have this for strings */
		AppendOutput('\0', &act->Cur->ot_head);
        
		#ifdef PRINT
			printf(". First char %c, number of chars %lu, for state '%c' @ %p\n", act->Cur->ot_head->c, col, act->Cur->stc, act->Cur);
			printf("---inserted pattern num %lu into the trie---\n", row);	
		#endif
		
		/* reset to root to build on further */
		act->Cur = act->Root;
	
	}

	#ifdef PRINT
		printf("\n~~~Goto Structure + Output Build Completed~~~\n");
	#endif
    
    return retVal;
}


/* Build the failure traces into a AC_Trie structure. These are allow the algorigthm 
*  to successfully process in an O(n) fashion where n is the size of the processed 
*  data. 
*
*  Essentially, to add the failtraces, BFS through Trie, looking at previous level 
*  for applicable failure states.
*
*  return-
*  (unsinged char): whether or not building the failure traces encountered an error 
*
*  params-
*
*  act (*): the AC_Trie structure that we are adding failure traces to.
*
*/
unsigned char BuildACTrieFailTraces(struct AC_Trie *act){
    
    unsigned char retVal = FALSE;
	
	#ifdef PRINT
		printf("\n~~~Building Failure Traces into Trie + Output~~~\n\n");
	#endif

	/* fifo queue used to traverse trie */
	struct FifoSteQ *faQ = malloc(sizeof(struct FifoSteQ));
	/* failed allocation */
	if(faQ == NULL){
		printf("ERROR BUILD #6\n");
		retVal = 1;
	}

	/* Set the initial failure state of level one states to the root states.
	*  Add them to the failure addition queue to be considered as failure states 
	*  for states at the next level of the trie. */	

	/* first, first level state is child of the root state */
	struct State *qAddSt = act->Root->cState;
	qAddSt->fState = act->Root;
	unsigned char faQinit = InitFifoSteQ(faQ, qAddSt);
    if(faQinit == FALSE){
        printf("ERROR BUILD #11\n");
        retVal = 1;
    }

	/* push the sibling states (the other first level states) */
	while(qAddSt->sState != NULL){
		
		#ifdef PRINT
			printf("Failure Q: Added state \"%c\" @ %p to the queue\n", qAddSt->stc, qAddSt);
		#endif

		qAddSt = qAddSt->sState;
		
		qAddSt->fState = act->Root;
		PushFifoSteQ(faQ, qAddSt);

	}
	
	#ifdef PRINT
		printf("Failure Q: Added state \"%c\" @ %p to the queue\n", qAddSt->stc, qAddSt);
	#endif
 
	/* when the head state is null, we processed every tree node*/
	while(faQ->head->stp != NULL){
		
		struct State *popSt = PopFifoSteQ(faQ);		  	 
		struct State *holdSt = popSt->cState;

		/* Traverse the current trie row column (all sibling states) */
		while(holdSt != NULL){

			/* add the next level states to the fifo queue*/
			PushFifoSteQ(faQ, holdSt);
			#ifdef PRINT
				printf("Failure Q: Added state \"%c\" @ %p to the queue\n", holdSt->stc, holdSt);
			#endif

			/* get and goto every failstate to see if it is a failstate
			*  for the node being tested for failstates */
			struct State *fSt = GetFailState(popSt); 	    
			act->Cur = fSt;

			/* this loop backtraces through the data structure to find the closest (level)
			*  failstate. The failstate is a point where the search could continue
			*  on a different branch, if the current branch is deemed not tennable.
			*  In other words, the failstate fails to the nearest possibility of 
			*  a match, whether it start over or continue on a different branch */
			while(ACgoto(act, holdSt->stc) == FALSE){

				/* break if we're the root node, it's the last possible fail point */
				if(act->Cur->stc == '\0'){
					break;
				}
				/* otherwise, continue to backtrack until we succeed in moving foward */
				else{
					fSt = GetFailState(fSt);
					act->Cur = fSt;
				}
			}

			/* whether it be the root node or a different branch, we have found our 
			*  next possible tennable branch, set the fail to that point */
			holdSt->fState = act->Cur;

			#ifdef PRINT
				printf(" \"%c\" set to fail at \"%c\" @ %p \n", holdSt->stc, holdSt->fState->stc, holdSt->fState);	
			#endif

			/* Take care of Output, we add if the fail state has an output */
			if(holdSt->fState->ot_head != NULL){
				
               CatOutput(holdSt->fState->ot_head, &holdSt->ot_head);
                
				#ifdef PRINT
					printf("\n first output char: '%c'\n", holdSt->ot_head->c);		
				#endif
			}

			/* we process through the trie level by level, so, move through siblings
			*  and after processing the last sibling, move to the next child in the 
			*  fifo queue */
			holdSt = holdSt->sState;
		
		}

	} 
	
	/* free queue memory */
	free(faQ);
	faQ = NULL;
	
	#ifdef PRINT
		printf("\n~~~Failure Traces + Output Built into the Trie~~~\n\n");
		printf("!!!!TRIE BUILD FINISHED!!!!\n\n");
	#endif

    return retVal;
}

/**
* Try to take a step in the given AC trie to the given char(state).
*
* returns - 
* (usigned char): whether or not we were able to go to the next state in the 
* AC_Trie
*
* params -
* act (AC_Trie *): the AC_Trie we are attempting to step through
* nxt (char): the character(State) we are attempting to travel to
*/
unsigned char ACgoto(struct AC_Trie *act, char nxt){

	unsigned char pass = FALSE;
	
	/* check current's child, if it has one, and all its siblings to see if we can proceed */
	if(act->Cur->cState != NULL){
		
		
		/* compare and move to child state if it's present and matching */
		struct State *cmpSt = act->Cur->cState;

		if(cmpSt->stc == nxt){
			
			pass = TRUE;
			act->Cur = cmpSt;
			
		}
		else{

			/* check the siblings for a match */
			while(cmpSt->sState != NULL){

				cmpSt = cmpSt->sState;
				
				/* if we found a match, break out of searching through siblings */
				if(cmpSt->stc == nxt){
					pass = TRUE;
					act->Cur = cmpSt;
					break;
				}
			
			}

		}

	}	


	return pass;

}

/**
* Get a state's failure trace location
*
* returns-
* (struct State *): the state that is listed as the current states fail state
*
* params-
* st (struct State *): the state we are getting the failstate of.
*/
struct State *GetFailState(struct State *st){
	return st->fState;
}

/*
* Search a given string with a given ACTrie for all patterns 
* in the ACTrie.
*
* returns-
* (unsigned long): the number of patterns found by the AC_Trie
*
* params-
* act( *): the AC_Trie (holds the patterns) that processes the input for pattern matches
* searchString(char *): the string to look for patterns in ~ input string
*/
unsigned long AC_Process(struct AC_Trie *act, char *searchString){

	unsigned long index = 0;
	unsigned long pattFound = 0;
	
	/* Always start at ROOT */
	act->Cur = act->Root;
	
	while(searchString[index] != '\0'){
				
		while((ACgoto(act, searchString[index]) == FALSE)){
			
			/* If we are the root at this point, we have failed 
			   back twice and need to move on */
			if(act->Cur == act->Root){
				break;
			}
			
			/* travel to current's fail state and try again */
			act->Cur = GetFailState(act->Cur);
		}		
		
		/* If we arrived at a state with an output, print it for now 
		   and keep track of the number of findings */
		if(act->Cur->ot_head != NULL){
			struct Output *tmp = act->Cur->ot_head;
            
			while(tmp != NULL){
                
				if(tmp->c == '\0'){
					pattFound++;
					
					#ifdef PRINT
						printf("\n");
					#endif
					
				}
				
				#ifdef PRINT
					printf("%c", tmp->c);
				#endif
				tmp = tmp->nxt;
				
			}
			
		}
		else{
			/* no output, continue */
		}
		
		index++;
	}

	return pattFound;
}	

/**
* Clean up an instance of the AC_Trie data type
*
* return-
*
* (unsigned char): whether or not the clean worked
*
* params-
*
* act (AC_Trie *): the trie container to free
*
*/
unsigned char CleanACTrie(struct AC_Trie *act){

	char retVal;

	#ifdef PRINT
		printf("freeing global/ACTrie\n");
	#endif

	if(act != NULL){
		
		retVal = FreeACTrie(act);		
		
		free(act);				
		
		act = NULL;
	}
	
	#ifdef PRINT
		printf("end freeing global/ACTrie\n\n");
	#endif

	return retVal;
}

/*
* Free a given AC trie structure by searching through the 
* trie and freeing via DFS
*
* returns-
*
* (unsigned char): whether or not the free had an error
*
* params-
* 
* act (AC_Trie *): the AC_Trie to free
*
*/
unsigned char FreeACTrie(struct AC_Trie *act){
	
	#ifdef PRINT
		printf("freeing AC trie\n");
	#endif
	
	unsigned char retVal = TRUE;
	
	/* Traverse the trie and free the states on pop */
	struct FifoSteQ *travQ = malloc(sizeof(struct FifoSteQ));	
	unsigned char inittest = InitFifoSteQ(travQ, act->Root);
	if(inittest != TRUE){
		retVal = FALSE;
	}
	
	/* process until we run out of states */
	while(travQ->head->stp != NULL){
		
		/* hold state for removal */
		struct State *killCSt;
		struct State *popSt = PopFifoSteQ(travQ);
		
		while(popSt->cState != NULL){
			
			killCSt = popSt;
			
			#ifdef PRINT
				printf("killing '%c'\n", killCSt->stc);
			#endif
			
			popSt = popSt->cState;			
			
			struct State *strav = popSt;
			while(strav->sState != NULL){
				
				strav = strav->sState;
				
				PushFifoSteQ(travQ, strav);
			}
			
			/* remove the output Linked List before the state */
			retVal += FreeOutput(killCSt->ot_head);
			free(killCSt);
			killCSt = NULL;
			
		}	
		
		
		killCSt = popSt;
		
		#ifdef PRINT
			printf("killing '%c'\n", killCSt->stc);
		#endif
		
		/* remove the output Linked List before the state */
		retVal += FreeOutput(killCSt->ot_head);
		free(killCSt);
		killCSt = NULL;
	}
	
	/* free the queue after it's been fully popped */
	free(travQ);
	travQ = NULL;
	
	
	return retVal;
}
