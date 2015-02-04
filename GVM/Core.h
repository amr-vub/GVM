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
#include "Globals.h"
#include <list>

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
	Core(int corid);
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
	list<Token_Type*>::iterator eraseToken(list<Token_Type*>::iterator index);

	// get the schedualed element of the queue and erase it afterwards
	Token_Type* getScheduleElement();

	// fields part (for simplicity, I will make them public)
	TokenDispatcher dispatcher;

	MatchingUnit matchUnit;

	Scheduler sch;

	IMemory memory;

	Tokenizer tokenizer;

	ContextCreater conxObj;

	// core id
	short coreID;	

	// list of direct neigboor Ids
	short nieghborList[NIEGHBOORS];

	// activate/ deactivate
	bool active;

	// some profiling data
	unsigned long long Idle_Counter;

	// Token queue
	list<Token_Type*> inbox;

	boost::mutex c_mutex;

private:
	// the thread that will run this core
	boost::thread c_thread;
	
};

