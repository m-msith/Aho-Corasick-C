/**
* IO.c - By: Martin Smith
*
* Functions held in this file are responsible for the simple IO bound functionality of the AC program, such
* as reading patterns, reading in collection data, as well as writing output to ahooutput.
*/

#include "IO.h"
#include "DefLib.h"

/*
* IOinP is responsible for reading through the patterns.txt file and returning in memory representations
* of the patterns found in the file. patterns.txt is made to contain a newline dilimitaed list of patterns.
* to build into the trie.
*/
char **IOinP(int *patNum){

	char tmpC;
	int  lngst = 0, tmpLen = 0;

	char **pats;
	int *nPats = patNum;

	/* obtain file metadata */
	FILE *fp = fopen("files.d/patterns.txt", "r");

	if(fp == NULL){
		printf("Error during pattern import #1\n");
		return NULL;
	}

	while(tmpC != EOF){
		tmpLen++;
		tmpC = fgetc(fp);
		if((tmpC == '\n') || (tmpC == '\r')){
			*nPats = *nPats + 1;

			if(tmpLen > lngst){
				lngst = tmpLen;
				tmpLen = 0;
			}
		}
	}

	/* add room for NULL */
	lngst += NTS; 

	/* seek start for read */
	rewind(fp); 

	if(fp == NULL){
		printf("Error during pattern import #2\n");
		return NULL;
	} 

	/* plus one for null ref string */
	*nPats = *nPats + 1; 

	pats = malloc(sizeof(char*) * (*nPats));

	int i = 0;
	for(i = 0; i < *nPats; i++){
		
		/* init for largest possible member  */
		pats[i] = malloc(sizeof(char) * lngst + NTS); 
		
		/* fgets adds NULL at end of imported string */
		fgets(pats[i], lngst + NTS, fp); 

		/* get rid of newline */
		int j = 0;	
		for(j = 0; j < lngst+NTS; j++){
			if((pats[i][j] == '\n') || (pats[i][j] == '\r')){
			pats[i][j] = '\0';
			}
		}
	}

	pats[*nPats] = malloc(sizeof(char));
	/* set last entry to null, functioning as collection terminator */
	pats[*nPats] = "\0"; 

	fclose(fp);

	return pats;
}
