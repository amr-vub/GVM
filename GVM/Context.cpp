/** 
* Context.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Context.h"


//intializing the static var
//int Context::conxCounter = -1;

Context::Context(short &coID, int &conxCounter)
{
	this->coreId = coID;	
	this->conxId = 0;	
	this->conx = conxCounter;
	generateUniqueConx();
}

Context::Context(void)
{
}


Context::~Context(void)
{
}

/*
* Function : generateUniqueConx()
* This function is used to generate a unique context Id based on
* the context's object core & conx values
*/
void Context::generateUniqueConx()
{
	//put the core value in the MSB of conxId
	this->conxId = this->coreId;
	this->conxId <<= 24;

	// now we OR the conx with the conxId 
	// so conxId will be like a pair (coreId, conx)
	long temp = 0;
	temp = conx;
	this->conxId |= temp;
}

/*	ContextCreater	*/

int ContextCreater::conxCounter = 0;

ContextCreater::ContextCreater()
{	
}

ContextCreater::~ContextCreater()
{
	// loop through avaliable to delete pointers
	while (!this->cxPool.empty()){
		Context* temp = this->cxPool.back();
        this->cxPool.pop_back();
        delete temp;
    }	
}

// return a unique context
Context* ContextCreater::getUniqueCx(short &corId)
{
	// if there exist a free already created context, return it
	if(!this->cxPool.empty())
	{
		Context *cx = this->cxPool.back();	
		// delete it as it's already taken
		this->cxPool.pop_back();
		return cx;
	}
	else
		return new Context(corId, ++this->conxCounter);
}

// free an already created context and add it to the avaliable vector
void ContextCreater::freeContext(Context *cx)
{
	this->cxPool.push_back(cx);
}