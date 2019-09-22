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

  #ifdef PRINT
    printf("~~~Global Init Pass~~~\n");
  #endif

  int patNum = 0;
  char **patterns = IOinP(&patNum);
  if(patterns == NULL){
	printf("ERROR Init #2\n");
	return 1;
  }

  g->NumPats = patNum;

  #ifdef PRINT
    printf("~~~IO Init Pass~~~\n");
  #endif

  int badRetBuild = BuildACTrie(patterns, g);
  if(badRetBuild == TRUE){
	printf("ERROR Init #3\n");
	return 1;
  }
  
  #ifdef PRINT
    printf("~~~Build Init Pass~~~\n");
  #endif

  #ifdef PRINT
    printf("~~~testing patterns~~~\n");
    testPatStore(patterns);
  #endif

  #ifdef PRINT
  printf("\"The Curse of Wandering permeated Amonkhet long before Nicol Bolas. He simply harnessed it.\"\n");
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

  //init root
  DefaultStateInit('\0', 0, g->Root);

  //root node is null and fails to itself
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
void ACgoto(Globals *g, char nxt){

  char pass = FALSE;

  //check throuh current's child and all its siblings to see if we can proceed    
  State *cmpSt = g->Cur->cState;

  do{
	
	//if we find a match exit loop
	if(cmpSt->stc == nxt){
	  g->Cur = cmpSt;
	  pass = TRUE;
	  break;	
	}
	//move to the child's sibling state
	else{		
	  cmpSt = cmpSt->sState;
	}

  }while(cmpSt != NULL);

  //goto fail state if we can't move on
  if(pass){
	//we already moved
  }
  else{	
	//goto fail state
	g->Cur = g->Cur->fState;
  }

}
