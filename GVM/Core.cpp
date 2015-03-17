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
	//this->active = true;
	int fake= 0;
	bool StartUp_Done = false;

	while(this->active)
	{			
		if(this->coreID == 0)
			fake++;
		if(this->inbox.size() != 0){
			/*
			if(this->Idle_Counter < this->inbox.size())
				this->Idle_Counter = this->inbox.size();*/						
			Token_Type *tok = this->getScheduleElement();//this->inbox.front();
			//this->inbox.pop_back();			
			if(tok->tag->instAdd[0] == 1 && tok->tag->instAdd[1] == 13)
				cout<< "";
			this->dispatcher.dispatch(tok);
			//this->eraseToken();			
		}
	}
	/*
	boost::lock_guard<boost::mutex> guard(s_mutex);
	cout << "core number: " << this->coreID << " was idle: " << fake << endl;
	*/
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
Token_Type* Core::getScheduleElement()
{	
	boost::lock_guard<boost::mutex> guard(c_mutex);
	
	Token_Type* temp = this->inbox.front();
	this->inbox.pop_front();

	return temp;
}