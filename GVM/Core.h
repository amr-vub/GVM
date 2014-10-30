/** 
* Core.h
* Amr Gamil
* GVM
*/

#pragma once
//#include <vector>
#include "Token.h"
#include "Context.h"
#include "TokenDispatcher.h"
#include "MatchingUnit.h"
#include "Scheduler.h"
#include "IMemory.h"

#include "Tokenizer.h"
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
	Core(int corid);
	Core();
	~Core();
	Core(Core const &obj) { /* FIXME */ abort(); }

	void start();

	// fields part (for simplicity, I will make them public)
	TokenDispatcher dispatcher;

	MatchingUnit matchUnit;

	Scheduler sch;

	IMemory memory;

	Tokenizer tokenizer;

	ContextCreater conxObj;

	// core id
	short coreID;

	static bool active;

	// Token queue
	static vector<Token<int>*> inbox;

};

