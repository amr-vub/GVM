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
void Scheduler::executeTwo(Token_Type** toks)
{
	//getting the tokens from 

	// getting the dest inst add
	int *instAdd = toks[0]->tag->instAdd;	

	Instruction* inst = this->core->memory.get(instAdd);

	inst->execute(toks, this->core);
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

	Token_Type* tokArr[1] = {&tok};

	inst->execute(tokArr, this->core);
}