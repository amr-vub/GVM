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
#include "IMemory.h"

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
	this->dispatcher = new TokenDispatcher();
	this->matchUnit = new MatchingUnit();
	this->sch = new Scheduler();
	this->tokenizer = new Tokenizer();
	this->conxObj = ContextCreater();
	this->memory = new IMemory();
	this->coreID = corid;	
	//this->active = false;

	this->dispatcher->core = *this;
	this->matchUnit->core = *this;
	this->sch->core = *this;
	this->tokenizer->core = *this;

	// very fake
	this->tokenizer->swicther = new Switcher();
	this->tokenizer->contextManager = new ContextManager();
	this->tokenizer->swicther->tokenizer = *this->tokenizer;
	this->tokenizer->contextManager->tokenizer = *this->tokenizer;

}

Core::Core(void)
{
}


Core::Core(Core& obj) 
{
	printf("Copy constructor allocating ptr. \n");
	coreID = obj.coreID;
	active = obj.active;
	tokenizer = new Tokenizer();
	*tokenizer = *obj.tokenizer;
	dispatcher = new TokenDispatcher();
	*dispatcher = *obj.dispatcher;
	matchUnit = new MatchingUnit();
	*matchUnit = *obj.matchUnit;
	sch = new Scheduler();
	*sch = *obj.sch;
}
//destructor
Core::~Core(void)
{

	delete dispatcher;
	delete matchUnit;
	delete sch;
	delete memory;
	delete tokenizer;
	//delete conxObj;
}

void Core::start()
{
	this->active = true;
	while(this->active)
	{	
		if(!this->inbox.empty()){
			Token<int> *tok = this->inbox.front();
			this->dispatcher->dispatch(tok);
			this->inbox.erase(this->inbox.begin());
			//delete tok;
		}
		else
			break;
	}
}