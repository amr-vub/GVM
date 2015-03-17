/** 
* Tag.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Tag.h"

// constructor
Tag::Tag(Context &cx, short prt, int *instAdd)
{
	this->conx = cx;
	this->port = prt;
	this->instAdd[0] = instAdd[0];
	this->instAdd[1] = instAdd[1];	
	this->instIdx = 0;
	this->tokenID = 0;
	generateUniqeInstIdx();
	generateUniqeMix();

}

Tag::Tag(void)
{
}

// destructor
Tag::~Tag(void)
{
}

//generate unique mix of  instAdd {chunk and mem add}
void Tag::generateUniqeInstIdx()
{
		//put the chunk value in the MSB of instIdx
	this->instIdx = this->instAdd[0];
	this->instIdx <<= 16;

	// now we OR the mem Add with the instIdx 
	// so instIdx will be like a pair (chunk, memAdd)
	long temp = 0;
	temp = this->instAdd[1];
	this->instIdx |= temp;
}

// generate unique mix between the cx unique id and instIdx to ease token key search
void Tag::generateUniqeMix()
{
	unsigned long long temp = 0;

	// add the inst Id in the most significant 32 bits
	this->tokenID = this->instIdx;
	this->tokenID <<= 32;

	// now we OR the mem Add with the instIdx 
	// so instIdx will be like a pair (chunk, memAdd)	
	temp = this->conx.conxId;
	this->tokenID |= temp;
}
