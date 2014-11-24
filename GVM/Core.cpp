/** 
* Core.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Core.h"

// constructor
Core::Core(int corid)
{
	this->active = false;
	this->coreID = corid;

	this->dispatcher.core = this;
	this->matchUnit.core = this;
	this->sch.core = this;
	this->tokenizer.core = this;

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
			Token_Type *tok = this->inbox.front();
			//this->inbox.pop_back();			
			this->dispatcher.dispatch(tok);
			this->inbox.erase(this->inbox.begin());			
		}
		else
			break;
	}
}