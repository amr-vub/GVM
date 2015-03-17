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
	unsigned long instIdx;

	// unique mix of cx id and instAdd
	unsigned long long tokenID;

public:
	Tag(Context &cx, short prt, int *instAdd);
	Tag(void);
	~Tag(void);
	//generate unique id for instAdd {chunk and mem add}
	void generateUniqeInstIdx();
	// generate unique mix between the cx unique id and instIdx to ease token key search
	void generateUniqeMix();
};

