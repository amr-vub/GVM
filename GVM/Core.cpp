/** 
* Core.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Core.h"
#include "TokenDispatcher.h"
#include "MatchingUnit.h"
#include "Scheduler.h"
#include "Tokenizer.h"

// constructor
Core::Core(IMemory* mem, int corid)
{
	dispatcher = new TokenDispatcher(*this);
	matchUnit = new MatchingUnit(*this);
	sch = new Scheduler(*this);
	memory = mem;
	tokenizer = new Tokenizer(*this);	
	coreID = corid;
}

Core::Core(void)
{
}

//destructor
Core::~Core(void)
{
	delete dispatcher;
	delete matchUnit;
	delete sch;
	delete memory;
	delete tokenizer;
	delete conxObj;
}

//copy constructor
Core::Core(const Core &obj)
{
	//TokenDispatcher* tok = new TokenDispatcher();
}

