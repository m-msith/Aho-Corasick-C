/**
* Functions for the manipulation and control of states.
*
* Code: Martin
*/

#include "State.h"

/*
* Init a default state value (this should go in state)
*/
void DefaultStateInit(char c, int id, State *st){
	  
  st->stc = c; 
  st->id = id;
  st->cState = NULL;
  st->sState = NULL;
  st->fState = NULL;
  st->ot = NULL;
  st->ot_head = NULL;
}
