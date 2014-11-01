/** 
* TokenDispatcher.h
* Amr Gamil
* GVM
*/
#pragma once
#include "Token_Type.h"

class Core;
/*
* This class represents the first element in the GVM pipleine,
* the TokenDispatcher, which is responsible for dispatching tokens
* from the token queue to the next step in the GVM pipeline, the
* context matcher.
* TODO: think about removing it!
*/
class TokenDispatcher
{

public:
	TokenDispatcher();
	~TokenDispatcher(void);	

	void dispatch(Token_Type *tok);

	// fields part
	Core *core;
};

