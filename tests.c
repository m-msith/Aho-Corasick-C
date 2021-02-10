/**
*Contains various tests to run
*/


#include "tests.h"
/*
* Ensure that the globals value inits properly
*/
char testGlobalInit(){
	
	char pf = TRUE;
	
	Globals *tg = InitGlobals();
	if(tg == NULL){
		printf("ERROR testGlobalInit: global allocation failure\n");
		return FALSE;
	}
	
	if(tg->Root != tg->Cur){
		pf = FALSE;
		printf("ERROR testGlobalInit: current not set to root\n");
	}
	
	if(tg->Root->fState != tg->Root){
		pf = FALSE;
		printf("ERROR testGlobalInit: root not set to fail to root\n");
	}
	
	if(tg->Root->stc != '\0'){
		pf = FALSE;
		printf("ERROR testGlobalInit: root character not null\n");
	}
	
	if(tg->Root->ot != NULL){
		pf = FALSE;
		printf("ERROR testGlobalInit: root output not null\n");
	}
	
	if(tg->Root->ot_head != NULL){
		pf = FALSE;
		printf("ERROR testGlobalInit: root output head not null\n");
	}
	
	if(tg->IDCount != 0){
		pf = FALSE;
		printf("ERROR testGlobalInit: id count not zero\n");
	}

	if(pf == TRUE){
		printf("Test passed, no issues!\n");		
	}
	
	return pf;
}

char testPatStore(char **pats){

	char pf = TRUE;

	char *cmpstr = "123451starnix0112e3j[asdf[v;jhlsdu82322aB";
	unsigned char cmpstridx = 0;	
	
	unsigned char row = 0;
	
	for(row = 0; row < 5; row++){ 

		/* reset counting var */
		unsigned char col = 0;
	
		while(pats[row][col] != '\0'){
			
			if(cmpstr[cmpstridx] != pats[row][col]){
				pf = FALSE;
				printf("ERROR testPatStore: nonmatching char %c\n", *pats[cmpstridx]);
			}
			
			col++;
			cmpstridx++;
		}
		
	}
	
	if(cmpstridx != 41){
		pf = FALSE;
		printf("ERROR testPatStore: processed more or less characters than should be in tree: \n");
		printf("%d instead of 41\n", cmpstridx);
	}
	
	if(pf == TRUE){
		printf("Test passed, no issues!\n");		
	}
	
	return pf;
}

/* Test that the build function is able to make 
   a correct pattern trie and that the ac process 
   function is able to use the trie to find the 
   patterns in the given input*/
char testTrieNovelBuild(){
	
	char pf = TRUE;
	
	Globals *tg = InitGlobals();
	if(tg == NULL){
		pf = FALSE;
		printf("ERROR testTrieNovelBuild: Global init\n");
	}
	
	tg->NumPats = 5;
	
	char patts[5][5] = 
	{
		
		{'a', 'b', 'c', 'd', '\0'},
		{'f', 'a', 'd', 'e', '\0'},
		{'d', 'e', '\0', '\0', '\0'},
		{'a', 'b', 'd', 'e', '\0'},
		{'\0', '\0', '\0', '\0', '\0'}
		
	};	
	
	char **pattz;
	pattz = malloc(sizeof(char *) * 5);
	/* allocation check */
	if(pattz == NULL){
		printf("ERROR testTrieNovelBuild: malloc failure 1\n");
		return FALSE;
	}
	
	int i, j;
	for(i = 0; i < 5; i++){
		
		pattz[i] = malloc(sizeof(char) * 5);
		/* allocation check */
		if(pattz[i] == NULL){
			printf("ERROR testTrieNovelBuild: malloc failure 2\n");
			return FALSE;
		}
		
		for(j = 0; j < 5; j++){
			pattz[i][j] = patts[i][j];
		}
	}
	
	char badRetBuild = BuildACTrie(pattz, tg);
	if(badRetBuild == TRUE){
		pf = FALSE;
		printf("ERROR testTrieNovelBuild: Trie Build Failed!\n");
	}
	
	char *testString = "abcd fade de abde abdede xxxx fadx abcefa abcde";
	
	printf("patterns for matching:\n\n");
	printf("%s\n",pattz[0]);
	printf("%s\n",pattz[1]);
	printf("%s\n",pattz[2]);
	printf("%s\n",pattz[3]);
	
	printf("\nTest Input: %s\n\n", testString);
	printf("Patterns found in test input, in order (should be 11):\n\n");
	
	unsigned int pattsFound = AC_Process(tg, testString);
	
	printf("\npattsFound: %d\n", pattsFound);
	
	if(pattsFound != 11){
		pf = FALSE;
		printf("ERROR testTrieNovelBuild: Wrong number of patterns found!\n");
	}
	
	CleanGlobals(tg);
	
	if(pf == TRUE){
		printf("Test passed, no issues!\n");		
	}
	
	return pf;
}

char testTrieLongStringsBuild(){
	/* 
	Globals *tg = InitGlobals();
	if(g == NULL){
		printf("ERROR Init #1\n");
		return 1;
	}
	
	tg->NumPats = patNum;
	
	patts[][]{
		
		
	};
	
	char badRetBuild = BuildACTrie(patts, tg);
	if(badRetBuild == TRUE){
		printf("ERROR Init #3\n");
		return 1;
	}
	 */
	return FALSE;
}
