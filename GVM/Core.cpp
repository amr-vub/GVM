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
	bool StartUp_Done = false;

	while(this->active)
	{			
		if(this->inbox.size() != 0){			
			// StartUp_Done is true now as the inbox got at least one element from master core
			StartUp_Done = true;
			Token_Type *tok = this->getScheduleElement();
			this->dispatcher.dispatch(tok);		
		}
		else if(this->ind_Inbox.size() != 0)
		{
			// StartUp_Done is true now as the inbox got at least one element from master core
			StartUp_Done = true;
			Token_Type *tok = this->getScheduleElement_IndInbox();//this->inbox.front();				
			this->dispatcher.dispatch(tok);
		}			
		// check if this idleness is not becasue of the startup
		if(StartUp_Done)		
			// now this core is idle after getting intial work from the master core
			// then load balancing has to be started
			this->tokenizer.loadBalancer();									
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

// insert tokens into the core's independant queue in a safe way
void Core::insertToken_InIndInbox(Token_Type* tok)
{
	// first acquire the lock
	boost::lock_guard<boost::mutex> guard(ind_c_mutex);
	ind_Inbox.push_back(tok);
}


// erase tokens into the core's queue
list<Token_Type*>::iterator Core::eraseToken(list<Token_Type*>::iterator index)
{
	boost::lock_guard<boost::mutex> guard(c_mutex);
	return inbox.erase(index);
}

// get the schedualed element of the queue and erase it afterwards
// TODO
Token_Type* Core::getScheduleElement()
{	
	boost::lock_guard<boost::mutex> guard(c_mutex);
	
	Token_Type* temp = this->inbox.front();
	this->inbox.pop_front();

	return temp;
}

// get the schedualed element of the queue and erase it afterwards
// TODO
Token_Type* Core::getScheduleElement_IndInbox()
{	
	boost::lock_guard<boost::mutex> guard(ind_c_mutex);
	
	Token_Type* temp = this->ind_Inbox.front();
	this->ind_Inbox.pop_front();

	return temp;
}