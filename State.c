/**
* Functions for the manipulation and control of states.
*
* Code: Martin
*/

#include "State.h"

/*
* Init a default state value (this should go in state)
*/
void DefaultStateInit(char c, int id, struct State *st){

	st->stc = c; 
	st->id = id;
	st->cState = NULL;
	st->sState = NULL;
	st->fState = NULL;
	st->ot = NULL;
	st->ot_head = NULL;
	
}

/* Append an Output to a given Output structure */
char AppendOutput(char c, struct Output **o){    
    
    char retVal = TRUE;
    
    struct Output *tout;
    
    /* Create the initial node */
    if(*o == NULL){
       
        *o = malloc(sizeof(struct Output));
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

/* Append an Output to a given Output structure (by copy, 
*  could do by reference too, maybe mess with that later)
*/
char CatOutput(struct Output *from, struct Output **to){
    
    char retVal = TRUE;
    
    struct Output *tout;
    struct Output *fout = from;
        
    /* move the to structure to its add point */
        
    /* take care of a needing a new structure at first */
    if(*to == NULL){
        *to = malloc(sizeof(struct Output)); 
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
        
    tout->nxt = NULL;
    
    return retVal;
    
}

