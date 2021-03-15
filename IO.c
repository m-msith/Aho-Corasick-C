/**
* IO.c - By: Martin Smith
*
* Functions held in this file are responsible for the simple IO bound functionality of the AC program, such
* as reading patterns, reading in collection data, as well as writing output to ahooutput.
*/

#include "IO.h"
#include "DefLib.h"

/*
* IOinP is responsible for reading through a patterns .txt file and returning in memory representations
* of the patterns found in the file. patterns.txt is made to contain a newline dilimitaed list of patterns.
* to build into the trie.
*/
char **IOinP(int *patNum, char *fname){

	char tmpC = '\0';
	int  lngst = 0, tmpLen = 0;

	char **pats;
	int *nPats = patNum;

	/* obtain file metadata */
	FILE *fp = fopen(fname, "r");

	if(fp == NULL){
		printf("Error during pattern import #1\n");
		return NULL;
	}

	char prevchar = '\0';
	while(tmpC != EOF){
		tmpLen++;
		tmpC = fgetc(fp);
		if(((tmpC == '\n') || (tmpC == '\r')) && ((prevchar != '\n') && (prevchar != '\r'))){
			*nPats = *nPats + 1;

			if(tmpLen > lngst){
				lngst = tmpLen;
				tmpLen = 0;
			}
		}
		
		prevchar = tmpC;
	}

	/* add room for NULL */
	lngst += NTS; 

	/* seek start for read */
	rewind(fp); 

	if(fp == NULL){
		printf("Error during pattern import #2\n");
		return NULL;
	} 

	pats = malloc(sizeof(char*) * (*nPats));
	/* allocation failure check */
	if(pats == NULL){
		printf("Error during pattern import #3\n");
		return NULL;
	}
	
	unsigned long i = 0;
	tmpC = '\0';
	prevchar = '\0';
	
	/* init for largest possible member  */
	pats[i] = malloc(sizeof(char) * lngst + NTS); 
	/* allocation failure check */
	if(pats[i] == NULL){
		printf("Error during pattern import #4\n");
		return NULL;
	}
	
	while(tmpC != EOF){
		
		tmpC = fgetc(fp);
		
		if((tmpC == '\n') || (tmpC == '\r')){
		
			if((prevchar != '\n') && (prevchar != '\r')){
				
				i++;
				
				pats[i] = malloc(sizeof(char) * lngst + NTS); 
				/* allocation failure check */
				if(pats[i] == NULL){
					printf("Error during pattern import #5\n");
					return NULL;
				}
				
			}
			else{
				/* consecutive newline, skip */
			}

		}
		else{
			
			/* Strncat needs null terminated strings 
			   malloc init's char* values as null*/
			char *ptmpC = malloc(sizeof(char) * 2);
			/* allocation failure check */
			if(ptmpC == NULL){
				printf("Error during pattern import #6\n");
				return NULL;
			}
			
			ptmpC[0] = tmpC;
			strncat(pats[i], ptmpC, lngst + NTS);
			
		}
		
		prevchar = tmpC;
	}
	
	/* close the file, finished with it */
	fclose(fp);

	return pats;
}
