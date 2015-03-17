/** 
* Core.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Core.h"

boost::mutex Core::s_mutex;
bool Core::active = true;

// constructor
Core::Core(int corid)
{
	//this->active = false;
	this->coreID = corid;

	this->dispatcher.core = this;
	this->matchUnit.core = this;
	this->sch.core = this;
	this->tokenizer.core = this;

	this->Idle_Counter = 0;
	// for now, just a RING topoogy with 2direct nieghboors, left and right
	this->nieghborList[0] = ( corid - 1 ) % CORENUMBERS;
	if(this->nieghborList[0] < 0)
		this->nieghborList[0] += CORENUMBERS;

	this->nieghborList[1] = ( corid + 1 ) % CORENUMBERS;
	if(this->nieghborList[1] >= CORENUMBERS)
		this->nieghborList[1] = 0;

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
	while(this->active)
	{			
		if(this->inbox.size() != 0){							
			Token_Type *tok = this->getScheduleElement_LIFO();						
			this->dispatcher.dispatch(tok);			
		}
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
	inbox.push_back(tok);
}

// erase tokens into the core's queue
list<Token_Type*>::iterator Core::eraseToken(list<Token_Type*>::iterator index)
{
	boost::lock_guard<boost::mutex> guard(c_mutex);
	return inbox.erase(index);
}

// get the schedualed element of the queue and erase it afterwards
// TODO
Token_Type* Core::getScheduleElement_LIFO()
{	
	boost::lock_guard<boost::mutex> guard(c_mutex);
	
	Token_Type* temp = this->inbox.back();
	this->inbox.pop_back();

	return temp;
}

// get the schedualed element of the queue and erase it afterwards
Token_Type* Core::getScheduleElement_FIFO()
{	
	boost::lock_guard<boost::mutex> guard(c_mutex);
	
	Token_Type* temp = this->inbox.front();
	this->inbox.pop_front();

	return temp;
}