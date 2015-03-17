/** 
* Core.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Core.h"
#include <iostream>

void LOG(string outString)
{
	if(DEBUG)
	{
		cout<<outString;
	}
}

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
			Token_Type *tok = this->inbox.back();
			this->inbox.pop_back();
			Tag tag = *tok->tag;
			this->dispatcher.dispatch(tok);			
			/*
			if(tag.instAdd[0] == 1 && (tag.instAdd[1] == 11 || tag.instAdd[1]== 16 || tag.instAdd[1] == 22))
				std::cout<< "" <<endl;			
			if(this->inbox.size() == 1)
				std::cout<< "" <<endl;			
			*/
			//this->inbox.erase(this->inbox.begin());		
			LOG("\n");
		}
		else
			break;
	}
}