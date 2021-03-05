/**
*Contains various tests to run
*/


#include "tests.h"
/*
* Ensure that the globals value inits properly
*/
char testGlobalInit(){
	
	char pf = TRUE;
	
	struct Globals *tg = InitGlobals();
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
	
	struct Globals *tg = InitGlobals();
	if(tg == NULL){
		pf = FALSE;
		printf("ERROR testTrieNovelBuild: Global init\n");
	}
	
	tg->NumPats = 6;
	
	char patts[5][5] = 
	{
		
		{'a', 'b', 'c', 'd', '\0'},
		{'f', 'a', 'd', 'e', '\0'},
		{'d', 'e', '\0', '\0', '\0'},
		{'a', 'b', 'd', 'e', '\0'},
		{'a', 'b', '\0', '\0', '\0'}
		
	};	
	
	char **pattz;
	pattz = malloc(sizeof(char *) * 5);
	/* allocation check */
	if(pattz == NULL){
		printf("ERROR testTrieNovelBuild: malloc failure 1\n");
		return FALSE;
	}
	
	int i, j;
	for(i = 0; i < 6; i++){
		
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
    
	/* build the AC trie with previous defined input */
	char badRetBuild = BuildACTrie(pattz, tg);
	if(badRetBuild == TRUE){
		pf = FALSE;
		printf("ERROR testTrieNovelBuild: Trie Build Failed!\n");
	}
	
    /* string to search through */
	char *testString = "abcd fade de abde abdede xxxx fadx abcefa abcde";
	
	printf("patterns for matching:\n\n");
	printf("%s\n",pattz[0]);
	printf("%s\n",pattz[1]);
	printf("%s\n",pattz[2]);
	printf("%s\n",pattz[3]);
    printf("%s\n",pattz[4]);
	
	printf("\nTest Input: %s\n\n", testString);
	printf("Patterns found in test input, in order (should be 16):\n\n");
	
    /* Use the defined trie and input to test found outputs */
	unsigned int pattsFound = AC_Process(tg, testString);
	
	printf("\npattsFound: %d\n", pattsFound);
	
	if(pattsFound != 16){
		pf = FALSE;
		printf("ERROR testTrieNovelBuild: Wrong number of patterns found!\n");
	}
	
	CleanGlobals(tg);
	
	if(pf == TRUE){
		printf("\nTest passed, no issues!\n");		
	}
	
	return pf;
}

char testTrieStructure(){
	
	char pf = TRUE;
	
	struct Globals *tg = InitGlobals();
	if(tg == NULL){
		pf = FALSE;
		printf("ERROR testTrieStructure: Global init\n");
	}
	
	tg->NumPats = 6;
	
	char patts[5][5] = 
	{
		
		{'a', 'd', 'c', 'd', '\0'},
		{'f', 'a', 'd', 'e', '\0'},
		{'d', 'e', '\0', '\0', '\0'},
		{'a', 'b', 'd', 'e', '\0'},
		{'a', 'b', 'a', 'b', '\0'}
		
	};	
	
	char **pattz;
	pattz = malloc(sizeof(char *) * 5);
	/* allocation check */
	if(pattz == NULL){
		printf("ERROR testTrieStructure: malloc failure 1\n");
		return FALSE;
	}
	
	int i, j;
	for(i = 0; i < 6; i++){
		
		pattz[i] = malloc(sizeof(char) * 5);
		/* allocation check */
		if(pattz[i] == NULL){
			printf("ERROR testTrieStructure: malloc failure 2\n");
			return FALSE;
		}
		
		for(j = 0; j < 5; j++){
			pattz[i][j] = patts[i][j];
		}
	}
    
	/* build the AC trie with previous defined input */
	char badRetBuild = BuildACTrie(pattz, tg);
	if(badRetBuild == TRUE){
		pf = FALSE;
		printf("ERROR testTrieStructure: Trie Build Failed!\n");
	}
	
	
	
	/* Traverse the trie and store in array for easy compares */
	struct FifoSteQ *travQ = malloc(sizeof(struct FifoSteQ));	
	char inittest = InitFifoSteQ(travQ, tg->Root);
	if(inittest != TRUE){
		
	}	
	
	struct State *SteArr[16];	
	i = 0;
	
	while(travQ->head != NULL){
		
		struct State *popSt = PopFifoSteQ(travQ);		
		
		while(popSt->cState != NULL){
			
			SteArr[i] = popSt;
			i++;
			
			popSt = popSt->cState;
			
			struct State *strav = popSt;
			
			while(strav->sState != NULL){
				
				strav = strav->sState;
				
				PushFifoSteQ(travQ, strav);
				
			}
			
		}
		
		SteArr[i] = popSt;
		i++;
		
	}
	
	/* 
		{'a', 'd', 'c', 'd', '\0'},
		{'f', 'a', 'd', 'e', '\0'},
		{'d', 'e', '\0', '\0', '\0'},
		{'a', 'b', 'd', 'e', '\0'},
		{'a', 'b', 'a', 'b', '\0'}
	
	trie:
	
'\0'-a01 -d02 -c03 -d04
 	  |	   |
	  |	  b05 -d06 -e07
	  |	        |
 	  |	       a08 -b09
 	 f10 -a11 -d12 -e13 	
	  |
	 d14 -e15
	
	array:
	
	0    1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
	'\0' a01 d02 c03 d04 f10 a11 d12 e13 d14 e15 b05 d06 e07 a08 b09
	
	*/
	
	/* check that the fail traces are in-tact and correct */
	if(SteArr[0]->fState != SteArr[0]){
		pf = FALSE;
	}
	
	if(SteArr[1]->fState != SteArr[0]){
		pf = FALSE;
	}
	
	if(SteArr[2]->fState != SteArr[9]){
		pf = FALSE;
	}
	
	if(SteArr[3]->fState != SteArr[0]){
		pf = FALSE;
	}
	
	if(SteArr[4]->fState != SteArr[9]){
		pf = FALSE;
	}
	
	if(SteArr[5]->fState != SteArr[0]){
		pf = FALSE;
	}
	
	if(SteArr[6]->fState != SteArr[1]){
		pf = FALSE;
	}
	
	if(SteArr[7]->fState != SteArr[2]){
		pf = FALSE;
	}
	
	if(SteArr[8]->fState != SteArr[10]){
		pf = FALSE;
	}
	
	if(SteArr[9]->fState != SteArr[0]){
		pf = FALSE;
	}
	
	if(SteArr[10]->fState != SteArr[0]){
		pf = FALSE;
	}
	
	if(SteArr[11]->fState != SteArr[0]){
		pf = FALSE;
	}
	
	if(SteArr[12]->fState != SteArr[9]){
		pf = FALSE;
	}
	
	if(SteArr[13]->fState != SteArr[10]){
		pf = FALSE;
	}
	
	if(SteArr[14]->fState != SteArr[1]){
		pf = FALSE;
	}
	
	if(SteArr[15]->fState != SteArr[11]){
		pf = FALSE;
	}
	
	
	/* check that sibling strings are in-tact and correct*/
	//a->fd
	if(SteArr[1]->sState != SteArr[5]){
		pf = FALSE;
	}
	
	if(SteArr[5]->sState != SteArr[9]){
		pf = FALSE;
	}
	
	//d->b
	if(SteArr[2]->sState != SteArr[11]){
		pf = FALSE;
	}
	
	//d->a
	if(SteArr[12]->sState != SteArr[14]){
		pf = FALSE;
	}

	
	/* check that child strings are in-tact and correct*/
	
	//a->dcd
	for(i = 0; i < 4; i++){
		if(SteArr[i]->cState != SteArr[i+1]){			
			pf = FALSE;
		}
	}
	
	//f->ade
	for(i = 5; i < 8; i++){
		if(SteArr[i]->cState != SteArr[i+1]){
			pf = FALSE;
		}
	}
	
	//b->de
	for(i = 9; i < 10; i++){
		if(SteArr[i]->cState != SteArr[i+1]){
			pf = FALSE;
		}
	}
	
	//d->be
	for(i = 11; i < 13; i++){
		if(SteArr[i]->cState != SteArr[i+1]){
			pf = FALSE;
		}
	}
	
	//d->e
	for(i = 14; i < 15; i++){
		if(SteArr[i]->cState != SteArr[i+1]){
			pf = FALSE;
		}
	}
	
	
	
	return pf;
}
