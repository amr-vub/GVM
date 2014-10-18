/** 
* Context.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Context.h"


//intializing the static var
int Context::conxCounter = -1;

Context::Context(short coID)
{
	this->coreId = coID;	
	this->conxId = 0;
	conxCounter += 1;
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
	this->conxId <<= 32;

	// now we OR the conx with the conxId 
	// so conxId will be like a pair (coreId, conx)
	long temp = 0;
	temp = conx;
	this->conxId |= temp;
}
