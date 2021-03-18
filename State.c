/**
* Functions for the control of states.
*
* State.c - By: Martin Smith
*/

#include "State.h"

/*
* Init default state values
*
* params-
*
* c (char): the char that the state represents
* id (unsigned long): the numerical ID of the state
* st (struct State *): the state to initialize 
*
*/
void DefaultStateInit(char c, unsigned long id, struct State *st){

	st->stc = c; 
	st->id = id;
	st->cState = NULL;
	st->sState = NULL;
	st->fState = NULL;
	st->ot_head = NULL;
	
}

/* Append an Output to a given Output Linked List
*
* return-
*
* (unsigned char): failure or success of append
*
* params-
*
* c (char): the output character
* o (struct Output **): the output linked list to add to
*
**/
unsigned char AppendOutput(char c, struct Output **o){    
    
    unsigned char retVal = TRUE;
    
    struct Output *tout;
    
    /* Create the initial node */
    if(*o == NULL){
       
        *o = malloc(sizeof(struct Output));
		if(o == NULL){
			retVal = FALSE;
		}
        tout = *o;
        
    }else{
        
        tout = *o;
        
        /* Cycle to add point without clobbering original */
        while(tout->nxt != NULL){
            tout = tout->nxt;
        }
        
        tout->nxt = malloc(sizeof(struct Output));
        if(tout->nxt == NULL){
            retVal = FALSE;
        }
        
        tout = tout->nxt;
    }

    /* Set the Value */
    tout->c = c;
    tout->nxt = NULL;

    return retVal;
    
}

/* 
* Append an Output to a given Output structure (by copy, 
* could do by reference too, maybe mess with that later)
*
* return-
*
* (unsigned char): failure or success of cat
*
* params-
*
* from (struct Output **): the output Linked List to cat from
* o (struct Output **): the output linked list to cat to
*
*/
unsigned char CatOutput(struct Output *from, struct Output **to){
    
    unsigned char retVal = TRUE;
    
    struct Output *tout;
    struct Output *fout = from;
        
    /* move the to structure to its add point */
        
    /* take care of a needing a new structure at first */
    if(*to == NULL){
        *to = malloc(sizeof(struct Output)); 
		if(*to == NULL){
			retVal = FALSE;
		}
        tout = *to;        
    }
    /* otherwise cycle to the end of the to linked list */
    else{
        tout = *to;
        
        while(tout->nxt != NULL){            
            tout = tout->nxt;
        }
        
        tout->nxt = malloc(sizeof(struct Output));
        if(tout->nxt == NULL){
            retVal = FALSE;
        }
        tout = tout->nxt;
    }
    
    /* Add the from output to the to output */
    while(fout->nxt != NULL){  
        tout->c = fout->c;
        
        tout->nxt = malloc(sizeof(struct Output));
        if(tout->nxt == NULL){
            retVal = FALSE;
        }
        
        tout = tout->nxt;
        fout = fout->nxt;
    }
    
    /* last to null to terminate*/
    tout->c = '\0';
    tout->nxt = NULL;
    
    return retVal;
    
}

/* 
* Function to free an output linked list
*
* return-
*
* (unsigned char): failure or success of free 
*
* params-
*
* o (struct Output *): the output linked list to free
*
*/
unsigned char FreeOutput(struct Output *o){
	
	#ifdef PRINT
		printf("	Starting Freeing Output\n");
	#endif
	
	while(o != NULL){
		struct Output *killO = o;
		
		#ifdef PRINT
			printf("	killo '%c'\n", killO->c);
		#endif
		
		o = o->nxt;
		free(killO);
		killO = NULL;
	}
	
	#ifdef PRINT
		printf("	Done Freeing Output\n");
	#endif
	
	return FALSE;
	
}