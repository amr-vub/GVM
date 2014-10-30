/** 
* Tag.h
* Amr Gamil
* GVM
*/
#pragma once
#include "Context.h"
/**
* Tag is the token identifier, it has three fields
* \param 
		contx: the context of the token (dynamic, can be changed during runtime)
* \param
		port: which port of the destination instruction 
		should the token be forwarded
* \param
		inst: the destination instruction address
*/
class Tag
{
public:
	Context conx;
	short port;
	int instAdd[2];
	// unique mix of chunk and memory location
	long instIdx;

public:
	Tag(Context &cx, short prt, int *instAdd);
	Tag(void);
	~Tag(void);
	//generate unique mix of  instAdd {chunk and mem add}
	void generateUniqeMix();
};

