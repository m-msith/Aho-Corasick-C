/**
* Main function area for the AhoCorasick algorithm. Pipeline control.
*
* Code: Martin
*/

#include "AC.h"


/**
*Entry point for the Aho-Corasick program.
*/
int main(){

  Globals *g = InitGlobals();
  if(g == NULL){
	printf("ERROR Init #1\n");
	return 1;
  }

  char **patterns = IOinP();
  if(patterns == NULL){
	printf("ERROR Init #2\n");
	return 1;
  }

  #if PRINT
    printf("~~~testing patterns~~~\n");
    testPatStore(patterns);
  #endif

  printf("\"The Curse of Wandering permeated Amonkhet long before Nicol Bolas. He simply harnessed it.\"\n");

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
	free(g->Cur);
	free(g);
  }

}
