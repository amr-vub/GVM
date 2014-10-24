/** 
* Context.h
* Amr Gamil
* GVM
*/

#pragma once
#include <vector>

using namespace std;

/*
* This class define the context
* The tag that is associated with each token has dynamic and static parts.
* Context is part of the dynamic part. This class defines it's structure.
* What identify a context are:
	- coreID:  the Id of the core that this cotext is attached to
	- conx: the actual value for this id
*/
class Context
{
public:
	Context(short &coID, int &conxCounter);
	Context();
	~Context(void);

	void generateUniqueConx();

	/*	fields	*/
	// The Id of the core that conext is attached to
	short coreId;

	// the actual value of the context
	int conx;

	// The unique context index, that will be generating by the function
	// generateUniqueConx
	long conxId;

};

/*
 Handels the context creation and keeps a pool of already
 created free contexts
*/
class ContextCreater
{
public:
	ContextCreater();
	~ContextCreater();

	// return a unique context
	Context* getUniqueCx(short &corId);

	void freeContext(Context* cx);

	/*	fields	*/

	// counter for the number of context created 
	static int conxCounter;

	// holds the already used context, but free now 
	// in order not to keep generating context, we use
	// existing resources
	vector<Context*> cxPool;

};