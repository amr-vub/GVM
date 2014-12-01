/** 
* Core.h
* Amr Gamil
* GVM
*/

#pragma once
#include "Context.h"
#include "TokenDispatcher.h"
#include "MatchingUnit.h"
#include "Scheduler.h"
#include "IMemory.h"
#include "Tokenizer.h"
#include "Token_Type.h"

#include <queue>
#include<boost\thread\thread.hpp>
#include<boost\thread\thread_time.hpp>
#include<boost\thread\mutex.hpp>

/** 
* This class defines the Core.
* A core is the basic building block of the GVM.
* A core is one fully functional processing element,
* that encompasses:
	- Token Dispatcher
	- Matching unit
	- Scheduler
	- IMemory
	- Tokenizer
*/
class Core
{
public:
	Core(int corid, IMemory &__memory);
	Core();
	~Core();
	Core(Core const &obj) { /* FIXME */ abort(); }

	void start();

	// spawn the thread
	void run();

	// wait for the thread to finish
	void stop();

	// insert tokens into the core's queue
	void insertToken(Token_Type* tok);
	
	// erase tokens into the core's queue
	void eraseToken();

	// get the last element of the queue and erase it afterwards
	Token_Type* getLastElement();

	// fields part (for simplicity, I will make them public)
	TokenDispatcher dispatcher;

	MatchingUnit matchUnit;

	Scheduler sch;

	IMemory memory;

	Tokenizer tokenizer;

	ContextCreater conxObj;

	// core id
	short coreID;

	bool active;

	// Token queue
	queue<Token_Type*> inbox;

private:
	// the thread that will run this core
	boost::thread c_thread;
	boost::mutex c_mutex;
};

