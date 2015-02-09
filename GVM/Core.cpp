/** 
* Core.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Core.h"
#include <iostream>

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
			Tag tag = *tok->tag;
			//this->inbox.pop_back();	
			if(tag.instAdd[0] == 0 && (tag.instAdd[1] == 16))
				std::cout<< "" <<endl;
			this->dispatcher.dispatch(tok);
			if(this->inbox.size() == 1)
				std::cout<< "" <<endl;
			this->inbox.erase(this->inbox.begin());						
		}
		else
			break;
	}
}