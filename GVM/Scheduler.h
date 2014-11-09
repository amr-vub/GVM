/** 
* Scheduler.h
* Amr Gamil
* GVM
*/
#pragma once

#include "Token_Type.h"

class Core;

/** 
* This class defines the GVM scheduler, which is responsible for:
  - When to execute the an execution packet
  - How to execute it.
*/
class Scheduler
{
public:
	Scheduler();
	~Scheduler(void);
	// This function is resposible for executing 
	// the dest instruction on the recieved tokens,
	// then call the next element in GVM pipeline
	// "Tokenizer"
	void executeTwo(vector<Token_Type*> &toks);

	void execute(Token_Type &it);

	// fields
	Core *core;
};

