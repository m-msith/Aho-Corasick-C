/**
* Main function area for the Aho Corasick algorithm.
*
* AC.c - By: Martin Smith
*/
#include "AC.h"

/**
* Entry point for the Aho-Corasick program. Reads user file input and then
* either runs the algorithm or runs through the tests.
*/
int main(){

	char *fname = "";

	#ifdef TEST
		fname = "files.d/patterns_test.txt";
	#else
		fname = "files.d/patterns.txt";
	#endif

	unsigned long patNum = 0;
	char **patterns = IOinP(&patNum, fname);
	if(patterns == NULL){
		printf("ERROR Init #1\n");
		return 1;
	}

	#ifdef TEST
	
		unsigned long testsPassed = 0;
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
		printf("%lu tests passed out of %d\n", testsPassed, TOTAL_TESTS);
		
	#else
	
		#ifdef PRINT
			printf("~~~IO Init Pass~~~\n");
		#endif

		struct AC_Trie *ACT = malloc(sizeof(struct AC_Trie));		
		if(ACT == NULL){
			printf("ERROR Init #2\n");
			return 1;
		}
		
		ACT->NumPats = patNum;

		char badRetBuild = BuildACTrie(patterns, ACT);
		if(badRetBuild == TRUE){
			printf("ERROR Init #3\n");
			return 1;
		}

		#ifdef PRINT
			printf("~~~Build Init Pass~~~\n");		
			printf("\"The Curse of Wandering permeated Amonkhet long before Nicol Bolas. He simply harnessed it.\"\n");
		#endif
		
	#endif

	/* clean patterns structure */
	unsigned long i;
	for(i = 0; i < patNum + 1; i++){
		free(patterns[i]);
		patterns[i] = NULL;
	}
	free(patterns);
	patterns = NULL;

	return 0;
}