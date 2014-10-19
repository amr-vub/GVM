/** 
* Scheduler.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Scheduler.h"
#include "IMemory.h"
#include "Instruction.h"


Scheduler::Scheduler(Core &core)
{
	this->core = core;
}


Scheduler::~Scheduler(void)
{
}

// This function is resposible for executing 
// the dest instruction on the recieved tokens,
// then call the next element in GVM pipeline
// "Tokenizer"
// \param: the token array
void Scheduler::execute(Token<int> *toks)
{
	//getting the tokens from 
	Token<int> tok1 = toks[0];
	Token<int> tok2 = toks[1];

	// getting the dest inst add
	int *instAdd = tok1.tag.instAdd;	

	Instruction* inst = this->core.memory->get(instAdd);

	inst->execute(toks, this->core);
}

// This function is resposible for executing 
// the dest instruction on the recieved tokens,
// then call the next element in GVM pipeline
// "Tokenizer"
// \param: the token to be executed upon
void Scheduler::execute(Token<int> &toks)
{
	
}