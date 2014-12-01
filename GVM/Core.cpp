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

// Entry point for starting the core work
void Core::start()
{
	this->active = true;	
	while(this->active)
	{			
		if(!this->inbox.empty()){
			Token_Type *tok = this->getLastElement();//this->inbox.front();
			//this->inbox.pop_back();			
			this->dispatcher.dispatch(tok);
			//this->eraseToken();			
		}
		//else
			//boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

// spawn the thread
void Core::run()
{
	this->c_thread = boost::thread(&Core::start, this);
}

// wait for the thread to finish
void Core::stop()
{
	this->c_thread.join();
}

// insert tokens into the core's queue in a safe way
// TODO, till we use TBB concurrent vector
void Core::insertToken(Token_Type* tok)
{
	// first acquire the lock
	boost::lock_guard<boost::mutex> guard(c_mutex);
	inbox.push(tok);
}


// erase tokens into the core's queue
void Core::eraseToken()
{
	boost::lock_guard<boost::mutex> guard(c_mutex);
	inbox.pop();
}

// get the last element of the queue and erase it afterwards
// TODO
Token_Type* Core::getLastElement()
{	
	boost::lock_guard<boost::mutex> guard(c_mutex);
	
	Token_Type* temp = this->inbox.front();
	this->inbox.pop();

	return temp;
}