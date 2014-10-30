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
// \param: the token array
void Scheduler::executeTwo(Token<int> toks[2])
{
	//getting the tokens from 

	// getting the dest inst add
	int *instAdd = toks[0].tag->instAdd;	

	Instruction* inst = this->core->memory.get(instAdd);

	inst->execute(toks, this->core);
}

// This function is resposible for executing 
// the dest instruction on the recieved tokens,
// then call the next element in GVM pipeline
// "Tokenizer"
// \param: the token to be executed upon
void Scheduler::execute(Token<int> &tok)
{
	// getting the dest inst add
	int *instAdd = tok.tag->instAdd;	

	Instruction* inst = this->core->memory.get(instAdd);

	inst->execute(&tok, this->core);
}