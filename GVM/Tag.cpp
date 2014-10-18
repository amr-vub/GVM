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
void Tag::generateUniqeMix()
{
		//put the chunk value in the MSB of instIdx
	this->instIdx = this->instAdd[0];
	this->instIdx <<= 32;

	// now we OR the mem Add with the instIdx 
	// so instIdx will be like a pair (chunk, memAdd)
	long temp = 0;
	temp = this->instAdd[0];
	this->instIdx |= temp;
}

/*
// setters and getters

void Tag::setConx(int cx)
{
	conx = cx;
}
void Tag::setPort(short prt)
{
	port = prt;
}
void Tag::setInst(int ins)
{
	inst = ins;
}
int Tag::getConx(void)
{
	return conx;
}
short Tag::getPort(void)
{
	return port;
}
int Tag::getInst(void)
{
	return inst;
}
*/