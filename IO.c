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
char **IOinP(){

  char tmpC;
  int nPats = 0, lngst = 0, tmpLen = 0;

  //obtain file metadata
  FILE *fp = fopen("files.d/patterns.txt", "r");

  if(fp == NULL){
	printf("Error during pattern import #1\n");
	return NULL;
  }

  while(tmpC != EOF){
	tmpLen++;
	tmpC = fgetc(fp);
	if(tmpC == '\n'){
	  nPats++;

	  if(tmpLen > lngst){
		lngst = tmpLen;
		tmpLen = 0;
	  }
	}
  }

  rewind(fp);//seek start for read

  if(fp == NULL){
	printf("Error during pattern import #2\n");
	return NULL;
  }

  char **pats = malloc(sizeof(char*) * nPats+1);//plus one for null reference

  int i;

  for(i = 0; i < nPats; i++){
	pats[i] = malloc(sizeof(char) * lngst); //init for largest possible member
	fgets(pats[i], lngst, fp);	
  }

  pats[nPats] = malloc(sizeof(char));
  pats[nPats] = "\0"; //set last entry to null, functioning as collection terminator

  fclose(fp);

  return pats;
}
