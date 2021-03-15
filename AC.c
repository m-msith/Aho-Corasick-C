/**
* Main function area for the Aho Corasick algorithm.
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
		printf("BEGIN ALL TESTS\n");
        
		printf("\n	~~~START testing global init~~~\n\n");
		testsPassed += testGlobalInit();
		printf("\n	~~~END testing global init~~~\n\n");
		
		printf("\n	~~~START testing pattern input~~~\n\n");
		testsPassed += testPatStore(patterns);
		printf("\n	~~~END testing pattern input~~~\n\n");
		
		printf("\n	~~~START testing novel trie build~~~\n\n");
		testsPassed += testTrieNovelBuild();
		printf("\n	~~~END testing novel trie build~~~\n\n");
		
		printf("\n	~~~START testing novel trie structure pointer analysis~~~\n\n");
		testsPassed += testTrieStructure();
		printf("\n	~~~END testing novel trie structure pointer analysis~~~\n\n");
		
		printf("END ALL TESTS\n");
		printf("%d tests passed out of %d\n", testsPassed, TOTAL_TESTS);
		
	#else
	
		struct Globals *g = malloc(sizeof(struct Globals));		
		
		if(g == NULL){
			printf("ERROR Init #2\n");
			return 1;
		}
		InitGlobals(g);
		
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
void InitGlobals(struct Globals *g){	

	g->Root = malloc(sizeof(struct State));

	/* init root */
	DefaultStateInit('\0', 0, g->Root);

	/* root node is null and fails to itself */
	g->Root->fState = g->Root;
	g->Cur = g->Root;

	g->IDCount = 0;

}

/**
*Clean up an instance of the globals data type
*/
char CleanGlobals(struct Globals *g){

	char pf;

	#ifdef PRINT
		printf("freeing global/ACTrie\n");
	#endif

	if(g != NULL){
		
		pf = FreeACTrie(g);		
		
		free(g);				
		
		g = NULL;
	}
	
	#ifdef PRINT
		printf("end freeing global/ACTrie\n\n");
	#endif

	return pf;
}