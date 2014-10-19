/** 
* Scheduler.h
* Amr Gamil
* GVM
*/
#pragma once

#include "Core.h"
#include "Token.h"

/** 
* This class defines the GVM scheduler, which is responsible for:
  - When to execute the an execution packet
  - How to execute it.
*/
class Scheduler
{
public:
	Scheduler(Core &core);
	~Scheduler(void);
	// This function is resposible for executing 
	// the dest instruction on the recieved tokens,
	// then call the next element in GVM pipeline
	// "Tokenizer"
	void execute(Token<int> *it);

	void execute(Token<int> &it);

	// fields
	Core core;
};

