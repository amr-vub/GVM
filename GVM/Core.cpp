/** 
* Core.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Core.h"

vector<Token<int>*> Core::inbox;
bool Core::active = false;
// constructor
/*
Core::Core(IMemory *memory, int corid, TokenDispatcher *disp, 
		   MatchingUnit  *matchingUnit, Scheduler *sch , Tokenizer* tokenizer)
		   */
Core::Core(int corid)
	/*: dispatcher(new TokenDispatcher()), matchUnit(new MatchingUnit()),
sch(new Scheduler()), tokenizer(new Tokenizer())*/
{
	this->dispatcher = TokenDispatcher();
	this->matchUnit = MatchingUnit();
	this->sch = Scheduler();
	this->tokenizer = Tokenizer();
	this->conxObj = ContextCreater();
	this->memory = IMemory();
	this->coreID = corid;	
	//this->active = false;

	this->dispatcher.core = this;
	this->matchUnit.core = this;
	this->sch.core = this;
	this->tokenizer.core = this;

	// very fake
	/*
	this->tokenizer.swicther = new Switcher();
	this->tokenizer.contextManager = new ContextManager();
	this->tokenizer.swicther->tokenizer = *this->tokenizer;
	this->tokenizer->contextManager->tokenizer = *this->tokenizer;
	*/

}

Core::Core(void)
{
}

//destructor
Core::~Core(void)
{
}

void Core::start()
{
	this->active = true;
	while(this->active)
	{	
		if(!this->inbox.empty()){
			Token<int> *tok = this->inbox.front();
			this->dispatcher.dispatch(tok);
			this->inbox.erase(this->inbox.begin());
			//delete tok;
		}
		else
			break;
	}
}