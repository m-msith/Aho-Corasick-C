/**
* Main function area for the AhoCorasick algorithm. Pipeline control.
*
* Code: Martin
*/
#include "AC.h"


/**
* Entry point for the Aho-Corasick program.
*/
int main(){

	char *fname = "";

	#ifdef TEST
		fname = "files.d/patterns_test.txt";
	#else
		fname = "files.d/patterns.txt";
	#endif

	int patNum = 0;
	char **patterns = IOinP(&patNum, fname);
	if(patterns == NULL){
		printf("ERROR Init #1\n");
		return 1;
	}

	#ifdef TEST
	
		char testsPassed = 0;
		
		printf("\n	~~~START testing global input~~~\n\n");
		testsPassed += testGlobalInit();
		printf("\n	~~~END testing global input~~~\n\n");
		
		printf("\n	~~~START testing pattern input~~~\n\n");
		testsPassed += testPatStore(patterns);
		printf("\n	~~~END testing pattern input~~~\n\n");
		
		printf("\n	~~~START testing novel trie build~~~\n\n");
		testsPassed += testTrieNovelBuild();
		printf("\n	~~~END testing novel trie build~~~\n\n");
		
		printf("END ALL TESTS\n");
		printf("%d tests passed out of %d\n", testsPassed, TOTAL_TESTS);
		
	#else
	
		Globals *g = InitGlobals();
		if(g == NULL){
			printf("ERROR Init #2\n");
			return 1;
		}

		#ifdef PRINT
			printf("~~~Global Init Pass~~~\n");
		#endif
		
		g->NumPats = patNum;

		#ifdef PRINT
			printf("~~~IO Init Pass~~~\n");
		#endif

		char badRetBuild = BuildACTrie(patterns, g);
		if(badRetBuild == TRUE){
			printf("ERROR Init #3\n");
			return 1;
		}

		#ifdef PRINT
			printf("~~~Build Init Pass~~~\n");		
			printf("\"The Curse of Wandering permeated Amonkhet long before Nicol Bolas. He simply harnessed it.\"\n");
		#endif
		
	#endif

	return 0;
}



/**
*Take care of initializing the globals data structure to be used by the entire program
*/
Globals* InitGlobals(){

	Globals *g = malloc(sizeof(Globals));
	if(g == NULL){
		return NULL;
	}

	g->Root = malloc(sizeof(State));
	if(g->Root == NULL){
		return NULL;
	}

	/* init root */
	DefaultStateInit('\0', 0, g->Root);

	/* root node is null and fails to itself */
	g->Root->fState = g->Root;

	g->Cur = g->Root;
	if(g->Cur == NULL){
		return NULL;
	}

	g->IDCount = 0;

	return g;
}

/**
*Clean up an instance of the globals data type
*/
void CleanGlobals(Globals *g){

	if(g != NULL){
		
		free(g->Root);
		
		if(g->Root != g->Cur){
			free(g->Cur);
		}
		
		free(g);
	}

}

/**
*Take care of going to the next state in the Trie
*/
char ACgoto(Globals *g, char nxt){

	char pass = FALSE;
	
	/* check current's child, if it has one, and all its siblings to see if we can proceed */
	if(g->Cur->cState != NULL){
		
		/* printf("GOTO: current: %c, childstate of %c, moving to %c\n", g->Cur->stc, g->Cur->cState->stc, nxt);
		* move to child if it's present */
		State *cmpSt = g->Cur->cState;

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
*Travel to a state's failure trace location
*/
State *GetFailState(State *st){
	return st->fState;
}

unsigned int AC_Process(Globals *g, char *searchString){

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
			
			Output *tmp = g->Cur->ot_head;
			
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
