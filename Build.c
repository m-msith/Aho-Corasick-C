/** * Build.c
*
* This file is used to take care of building the AC Trie. The AC trie consists of three build steps: Building the 
* Trie structure, building the Fail traces into the structure, and building the output structure into each node
* of the Trie structure.
*
*/

#include "Build.h"
#include "AC.h"

/* 
* Trie Build function
* Returns: 1 if fail during building Trie structure, 2 if during building fail traces, 3 if during building output 
*/
char BuildACTrie(char **patts, Globals *g){

	char retVal = 0;
	int row = 0;
  
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
					
					g->Cur->cState = malloc(sizeof(State));

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

					g->Cur->sState = malloc(sizeof(State));

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
		g->Cur->ot = malloc(sizeof(Output));
		g->Cur->ot_head = malloc(sizeof(Output));
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
			
			g->Cur->ot->nxt = malloc(sizeof(Output));
			g->Cur->ot = g->Cur->ot->nxt;
			
			col++;
		}
	
		/* set to null to delimit, also good practice to have this for strings */
		g->Cur->ot->c = '\0';

		#ifdef PRINT
			printf(". First char %c, number of chars %d, for state '%c' @ %p\n", g->Cur->ot_head->c, col, g->Cur->stc, g->Cur);
		#endif

		#ifdef PRINT
			printf("---inserted pattern num %d into the trie---\n", row);	
		#endif
		
		/* reset to root for next pattern or task
		*  this reset only takes place after the ingestion
		*  of one pattern to properly build upon itself with the next one
		*  which will either share a branch or build a new one */
		g->Cur = g->Root;
	
	}

	#ifdef PRINT
		printf("\n~~~Goto Structure Build Completed~~~\n\n");
	#endif

	/** Build Fail Traces **/

	/* fifo queue used to traverse trie */
	FifoSteQ *faQ = malloc(sizeof(FifoSteQ));

	/* Set the initial failure state of level one states to the root states.
	*  Add them to the failure addition queue to be considered as failure states 
	*  for states at the next level of the trie. */	

	/* first push is the child of the root state */
	State *qAddSt = g->Root->cState;
	qAddSt->fState = g->Root;
	InitFifoSteQ(faQ, qAddSt);

	/* next push the sibling states of the root state child 
	*  as they are at the 'same level' */
	while(qAddSt->sState != NULL){
		
		#ifdef PRINT
			printf("Failure Q: Added state %c to the queue\n", qAddSt->stc);
		#endif

		qAddSt = qAddSt->sState;

		/* 1st level always fails to root node */
		qAddSt->fState = g->Root;
		PushFifoSteQ(faQ, qAddSt);

	}
	
	#ifdef PRINT
		printf("Failure Q: Added state %c to the queue\n", qAddSt->stc);
	#endif
 
	/* process queued states until there are no more, signifying 
	*  that we got through the whole trie */
	while(faQ->head != NULL){
		
		State *popSt = PopFifoSteQ(faQ);	

		/* visit each of the possible child states of the popped state */		  	 
		State *gtSt = popSt->cState;

		/* Traverse the current trie row, we added all rows during the 
		   first sibling loop, so no need to cycle through them */
		while(gtSt != NULL){

			/* add the next level states to the fifo queue
			*  this will get us through the entire queue
			*  as we cycle until empty */
			PushFifoSteQ(faQ, gtSt);

			/* get and goto every failstate to see if it is a failstate
			*  for the node being tested for failstates */
			State *fSt = GetFailState(popSt); 	    
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

				if(gtSt->ot == NULL){
					
					gtSt->ot = malloc(sizeof(Output));
					gtSt->ot_head = malloc(sizeof(Output));
					gtSt->ot_head = gtSt->ot;
					
				}

				Output *tmp = gtSt->fState->ot_head;

				#ifdef PRINT
					printf("appending output value: '%c'", gtSt->fState->ot_head->c);
				#endif

				/* Add fail trace outputs if any */
				while(tmp != NULL){

					#ifdef PRINT
						printf("%c", tmp->c);
					#endif

					gtSt->ot->c = tmp->c;

					tmp = tmp->nxt;

					gtSt->ot->nxt = malloc(sizeof(Output));
					gtSt->ot = gtSt->ot->nxt;

				}

				#ifdef PRINT
					printf("\n first output char: %c\n", gtSt->ot_head->c);
				#endif
			}

			/* we process through the trie level by level, so, move through siblings
			*  and after processing the last sibling, move to the next child in the 
			*  fifo queue */
			gtSt = gtSt->sState;
		
		}

	} 

	return retVal;

}

