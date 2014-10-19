/** 
* Core.h
* Amr Gamil
* GVM
*/

#pragma once
#include <vector>
#include "Token.h"

class TokenDispatcher;
class MatchingUnit;
class Scheduler;
class MatchingUnit;
class IMemory;
class Tokenizer;
class Context;
/** 
* This class defines the Core.
* A core is the basic building block of the GVM.
* A core is one fully functional processing element,
* that encompasses:
	- Token Dispatcher
	- Matching unit
	- Scheduler
	- IMemory
	- Tokenizer
*/
class Core
{
public:
	Core(IMemory* mem, int corid);
	Core(void);
	~Core(void);
	Core(const Core &obj);
	// fields part (for simplicity, I will make them public)
	TokenDispatcher* dispatcher;

	MatchingUnit* matchUnit;

	Scheduler* sch;

	IMemory* memory;

	Tokenizer* tokenizer;

	Context* conxObj;

	// core id
	int coreID;

	// Token queue
	//vector<Token<int>> inbox;

};

