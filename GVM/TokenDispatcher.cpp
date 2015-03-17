/** 
* TokenDispatcher.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Core.h"

TokenDispatcher::TokenDispatcher()
{
	//core = cr;
}

TokenDispatcher::~TokenDispatcher(void)
{
}
/*
* Takes a token from the token queue and dispatch it to the apporpriate 
* element in the GVM pipeline
* \param 
tok : the fetched token from the token queue
*/
void TokenDispatcher::dispatch(Token_Type *tok)
{
	// login some info for debugging
	LOG( "Dispatching Token with Context " +to_string(tok->tag->conx.conxId)  + 
		" instruction address [" + to_string(tok->tag->instAdd[0]) + "," + to_string(tok->tag->instAdd[1])+ "] Port:  " +
		to_string(tok->tag->port) + "\n" );			

	// getting instruction address
	int *instIdx = tok->tag->instAdd;
	// check if the token belongs to an dyatic instruction or monadic one
	// Instruction stord in chunk 0 -> monadic
	// Instruction stord in chunk 1 -> dyatic
	if(instIdx[0] == 0)
	{
		// call the schedual directly
		this->core->sch.execute(*tok);		
	}
	else
		// call the matching unit
		this->core->matchUnit.executeOrUpdateTable(tok);
	//core.coreID;
}