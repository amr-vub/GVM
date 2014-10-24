/** 
* Scheduler.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Scheduler.h"
#include "IMemory.h"
#include "Instruction.h"
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
	//Token<int>* tok1 = toks[0];
	//Token<int>* tok2 = toks[1];

	// getting the dest inst add
	int *instAdd = toks[0].tag->instAdd;	

	Instruction* inst = this->core.memory->get(instAdd);

	inst->execute(toks, this->core);

	//free the memory
	//delete toks[0];
	//delete tok2;
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

	Instruction* inst = this->core.memory->get(instAdd);
	/*
	vector<tuple<int*,short>> temp;
	this->core.tokenizer->wrapAndSend(temp, tok.data, tok.tag->conx);
	*/
//	cout<<this->core.active;
	inst->execute(&tok, this->core);
	//delete &tok;
}