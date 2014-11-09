/** 
* Scheduler.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Scheduler.h"
#include "IMemory.h"
#include "Instruction.h"
#include "Core.h"
#include <iostream>


Scheduler::Scheduler()
{
	//this->core = core;
}


Scheduler::~Scheduler(void)
{
}

// This function is resposible for executing 
// the dest instruction on the recieved tokens,
// then call the next element in GVM pipeline
// "Tokenizer"
// \param: the token vector
void Scheduler::executeTwo(vector<Token_Type*> &toks)
{
	//getting the tokens from 

	// getting the dest inst add
	int *instAdd = toks[0]->tag->instAdd;	

	Instruction* inst = this->core->memory.get(instAdd);

	Token_Type *tokensPacket = new Token_Type[toks.size()];
	// TODO: remove
	short idx = 0;
	for (vector<Token_Type*>::iterator it = toks.begin(); it!=toks.end();++it)
	{
		tokensPacket[idx++] = *(*it);
	}
	inst->execute(tokensPacket, this->core);
	//toks.clear();
	//try
	//delete [] tokensPacket;
}

// This function is resposible for executing 
// the dest instruction on the recieved tokens,
// then call the next element in GVM pipeline
// "Tokenizer"
// \param: the token to be executed upon
void Scheduler::execute(Token_Type &tok)
{
	// getting the dest inst add
	int *instAdd = tok.tag->instAdd;	

	Instruction* inst = this->core->memory.get(instAdd);

	inst->execute(&tok, this->core);
}