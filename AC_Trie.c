/** * AC_Trie.c
*
* This file is used to take care of building the AC Trie and running the AC algorithm. 
* 
* Building the AC trie consists of three build steps: Building the =Trie structure, 
* building the Fail traces into the structure, and building the output structure into each node
* of the Trie structure. Output building is taken care of at the end of building the structure 
* and at the end of building the fail traces. 
*
* Running through the AC trie requires an input string and an AC trie. The processing function
* goes through the input string char by char, and processes the next state of the trie to determine
* possible matches.
*/

#include "AC_Trie.h"
#include "SteQ.h"

/* 
* Trie Build function
* Returns: 1 if fail
*/
char BuildACTrie(char **patts, struct Globals *g){

	char retVal = 0;
	int row = 0;
  
	#ifdef PRINT
		printf("!!!!BUILDING TRIE!!!!\n\n");
		printf("~~~Building Goto Structure + Output~~~\n\n");
	#endif
  
	/* add a patterns, char at a time, skip last null entry */
	for(row = 0; row < (g->NumPats - 1); row++){ 

		/* reset counting var */
		int col = 0;
	
		while(patts[row][col] != '\0'){

			/* cycle through the trie to the first available new state point*/
			while(ACgoto(g, patts[row][col]) == TRUE){
		
				#ifdef PRINT	
					printf("skipping %c, already in the trie @ %p\n", patts[row][col], g->Cur);
				#endif
			
				col++;
			}	  
		
			/* add new element to the tree at the appropriate position if we didn't cycle to the last value */
			if(patts[row][col] != '\0'){
			  
				/* only ever one child element, else have to use the child's sibling element */
				if(g->Cur->cState == NULL){
					
					g->Cur->cState = malloc(sizeof(struct State));
					/* failed allocation */
					if(g->Cur->cState == NULL){
						printf("ERROR Build #1\n");
						return 1;		  
					}
						
					g->Cur = g->Cur->cState;		
				}
				/* go to child's sibling state if there is a child state already */
				else{
					
					g->Cur = g->Cur->cState;	  	

					/* cycle to the last sibling state to postpend */
					while(g->Cur->sState != NULL){
						g->Cur = g->Cur->sState;
					}

					g->Cur->sState = malloc(sizeof(struct State));
					/* failed allocation */
					if(g->Cur->sState == NULL){
						printf("ERROR BUILD #2\n");
						return 1;
					}

					g->Cur = g->Cur->sState;
				}
			  
				/* Init new state to the end of the trie */
				DefaultStateInit(patts[row][col], g->IDCount++, g->Cur);

				#ifdef PRINT	  
					printf("Adding state \"%c\" @ %p\n", g->Cur->stc, g->Cur);
				#endif


				col++;
			}
			else{
				/* We don't add a new state because the pattern is already in
				*  the trie, need to add the output though */
			}

		}

		/* reset col */
		col = 0;

		/* initialize output for state with output (last char in the pattern) */
		g->Cur->ot = malloc(sizeof(struct Output));
		/* failed allocation */
		if(g->Cur->ot == NULL){
			printf("ERROR BUILD #3\n");
			return 1;
		}
		
		g->Cur->ot_head = malloc(sizeof(struct Output));
		/* failed allocation */
		if(g->Cur->ot_head == NULL){
			printf("ERROR BUILD #4\n");
			return 1;
		}
		
		g->Cur->ot_head = g->Cur->ot;

		#ifdef PRINT
			printf("adding output: ");
		#endif

		/* Add the output */
		while(patts[row][col] != '\0'){
			
			g->Cur->ot->c = patts[row][col];
			
			#ifdef PRINT
				printf("%c", g->Cur->ot->c);
			#endif
			
			g->Cur->ot->nxt = malloc(sizeof(struct Output));
			/* failed allocation */
			if(g->Cur->ot->nxt == NULL){
				printf("ERROR BUILD #5\n");
				return 1;
			}
			
			g->Cur->ot = g->Cur->ot->nxt;
			
			col++;
		}
	
		/* set to null to delimit, also good practice to have this for strings */
		g->Cur->ot->c = '\0';
		
		/* set next to beeg null for easy processing*/
		g->Cur->ot->nxt = NULL;
		
		#ifdef PRINT
			printf(". First char %c, number of chars %d, for state '%c' @ %p\n", g->Cur->ot_head->c, col, g->Cur->stc, g->Cur);
			printf("---inserted pattern num %d into the trie---\n", row);	
		#endif
		
		/* reset to root for next pattern or task
		*  this reset only takes place after the ingestion
		*  of one pattern to properly build upon itself with the next one
		*  which will either share a branch or build a new one */
		g->Cur = g->Root;
	
	}

	#ifdef PRINT
		printf("\n~~~Goto Structure + Output Build Completed~~~\n");
	#endif

	/** Build Fail Traces **/

	#ifdef PRINT
		printf("\n~~~Building Failure Traces into Trie + Output~~~\n\n");
	#endif

	/* fifo queue used to traverse trie */
	struct FifoSteQ *faQ = malloc(sizeof(struct FifoSteQ));
	/* failed allocation */
	if(faQ == NULL){
		printf("ERROR BUILD #6\n");
		return 1;
	}

	/* Set the initial failure state of level one states to the root states.
	*  Add them to the failure addition queue to be considered as failure states 
	*  for states at the next level of the trie. */	

	/* first push is the child of the root state */
	struct State *qAddSt = g->Root->cState;
	qAddSt->fState = g->Root;
	char faQinit = InitFifoSteQ(faQ, qAddSt);
    /* make sure we got through init fine */
    if(faQinit == FALSE){
        printf("ERROR BUILD #11\n");
        return 1;
    }

	/* next push the sibling states of the root state child 
	*  as they are at the 'same level' */
	while(qAddSt->sState != NULL){
		
		#ifdef PRINT
			printf("Failure Q: Added state \"%c\" @ %p to the queue\n", qAddSt->stc, qAddSt);
		#endif

		qAddSt = qAddSt->sState;

		/* 1st level always fails to root node */
		qAddSt->fState = g->Root;
		PushFifoSteQ(faQ, qAddSt);

	}
	
	#ifdef PRINT
		printf("Failure Q: Added state \"%c\" @ %p to the queue\n", qAddSt->stc, qAddSt);
	#endif
 
	/* process queued states until there are no more, signifying 
	*  that we got through the whole trie */
	while(faQ->head != NULL){
		
		struct State *popSt = PopFifoSteQ(faQ);	

		/* visit each of the possible child states of the popped state */		  	 
		struct State *gtSt = popSt->cState;

		/* Traverse the current trie row, we added all rows during the 
		   first sibling loop, so no need to cycle through them */
		while(gtSt != NULL){

			/* add the next level states to the fifo queue
			*  this will get us through the entire queue
			*  as we cycle until empty */
			PushFifoSteQ(faQ, gtSt);
			#ifdef PRINT
				printf("Failure Q: Added state \"%c\" @ %p to the queue\n", gtSt->stc, gtSt);
			#endif

			/* get and goto every failstate to see if it is a failstate
			*  for the node being tested for failstates */
			struct State *fSt = GetFailState(popSt); 	    
			g->Cur = fSt;

			/* this loop backtraces through the data structure to find the closest (level)
			*  failstate. The failstate is a point where the search could continue
			*  on a different branch, if the current branch is deemed not tennable.
			*  In other words, the failstate fails to the nearest possibility of 
			*  a match, whether it start over or continue on a different branch */
			while(ACgoto(g,gtSt->stc) == FALSE){

				/* break if we're the root node, it's the last possible fail point */
				if(g->Cur->stc == '\0'){
					break;
				}
				/* otherwise, continue to backtrack until we succeed in moving foward */
				else{
					fSt = GetFailState(fSt);
					g->Cur = fSt;
				}
			}

			/* whether it be the root node or a different branch, we have found our 
			*  next possible tennable branch, set the fail to that point */
			gtSt->fState = g->Cur;

			#ifdef PRINT
				printf(" \"%c\" set to fail at \"%c\" @ %p \n", gtSt->stc, gtSt->fState->stc, gtSt->fState);	
			#endif

			/* Take care of Output, we add if the fail state has an output, and 
			if it's not the root node */
			if(gtSt->fState->ot_head != NULL){
				
				/* if no output already here, add new output 
				   otherwise, move to next position before adding*/
				if(gtSt->ot_head == NULL){
					
					gtSt->ot = malloc(sizeof(struct Output));
					/* failed allocation */
					if(gtSt->ot == NULL){
						printf("ERROR BUILD #7\n");
						return 1;
					}
					
					gtSt->ot_head = malloc(sizeof(struct Output));
					/* failed allocation */
					if(gtSt->ot_head == NULL){
						printf("ERROR BUILD #8\n");
						return 1;
					}
					
					gtSt->ot_head = gtSt->ot;
					
				}
				else{
					
					gtSt->ot->nxt = malloc(sizeof(struct Output));
					/* failed allocation */
					if(gtSt->ot->nxt == NULL){
						printf("ERROR BUILD #9\n");
						return 1;
					}
					
					gtSt->ot = gtSt->ot->nxt;
				}

				struct Output *tmp = gtSt->fState->ot_head;

				#ifdef PRINT
					printf("appending output values: ");
				#endif

				/* Add fail trace outputs if any */
				while(tmp->nxt != NULL){

					#ifdef PRINT
						printf("'%c' ", tmp->c);
					#endif

					gtSt->ot->c = tmp->c;

					tmp = tmp->nxt;

					gtSt->ot->nxt = malloc(sizeof(struct Output));
					/* failed allocation */
					if(gtSt->ot->nxt == NULL){
						printf("ERROR BUILD #10\n");
						return 1;
					}
		
					gtSt->ot = gtSt->ot->nxt;

				}				
				
				/* null terminate */
				gtSt->ot->c = '\0';
				/* set last to beeg null for easy looping */
				gtSt->ot->nxt = NULL;
				
				#ifdef PRINT
					printf("\n first output char: '%c'\n", gtSt->ot_head->c);		
				#endif
			}

			/* we process through the trie level by level, so, move through siblings
			*  and after processing the last sibling, move to the next child in the 
			*  fifo queue */
			gtSt = gtSt->sState;
		
		}

	} 
	
	#ifdef PRINT
		printf("\n~~~Failure Traces + Output Built into the Trie~~~\n\n");
		printf("!!!!TRIE BUILD FINISHED!!!!\n\n");
	#endif

	return retVal;

}

/**
*Take care of going to the next state in the Trie
*/
char ACgoto(struct Globals *g, char nxt){

	char pass = FALSE;
	
	/* check current's child, if it has one, and all its siblings to see if we can proceed */
	if(g->Cur->cState != NULL){
		
		/* printf("GOTO: current: %c, childstate of %c, moving to %c\n", g->Cur->stc, g->Cur->cState->stc, nxt);
		* move to child if it's present */
		struct State *cmpSt = g->Cur->cState;

		if(cmpSt->stc == nxt){
			
			pass = TRUE;
			g->Cur = cmpSt;
			
		}
		else{

			/* check the siblings for a match */
			while(cmpSt->sState != NULL){

				cmpSt = cmpSt->sState;
				
				/* if we found a match, break out of searching through siblings */
				if(cmpSt->stc == nxt){
					pass = TRUE;
					g->Cur = cmpSt;
					break;
				}
			
			}

		}

	}	


	return pass;

}

/**
*Get a state's failure trace location
*/
struct State *GetFailState(struct State *st){
	return st->fState;
}

unsigned int AC_Process(struct Globals *g, char *searchString){

	unsigned long index = 0;
	unsigned int pattFound = 0;
	
	/* Always start at ROOT */
	g->Cur = g->Root;
	
	while(searchString[index] != '\0'){
				
		while((ACgoto(g, searchString[index]) == FALSE)){
			
			/* If we are the root at this point, we have failed 
			   back twice and need to move on */
			if(g->Cur == g->Root){
				break;
			}
			
			/* travel to current's fail state and try again */
			g->Cur = GetFailState(g->Cur);
		}		
		
		/* If we arrived at a state with an output, print it for now 
		   and keep track of the number of findings */
		if(g->Cur->ot_head != NULL){
			
			struct Output *tmp = g->Cur->ot_head;
			
			while(tmp != NULL){
				
				if(tmp->c == '\0'){
					pattFound++;
					
					printf("\n");
					
				}
				
				printf("%c", tmp->c);
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

