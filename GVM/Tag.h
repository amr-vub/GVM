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

public:
	Tag(Context cx, short port);
	~Tag(void);
	/*
	void setConx(int cx);
	void setPort(short prt);
	void setInst(int ins);
	int getConx(void);
	short getPort(void);
	int getInst(void);
	*/
};

