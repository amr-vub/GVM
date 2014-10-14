/** 
* Tag.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Tag.h"

// constructor
Tag::Tag(int cx, short prt, int ins)
{
	conx = cx;
	port = prt;
	inst = ins;
}

// destructor
Tag::~Tag(void)
{
}

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