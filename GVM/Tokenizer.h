/** 
* Tokenizer.h
* Amr Gamil
* GVM
*/

#pragma once

#include <vector>
#include "Context.h"
#include "Core.h"

using namespace std;

/*
* This class defines the Tokenizer.
* The tokenizer is responsible for:
 - collecting the result out of an execution of an execution packet.
 - Wrap this result in a token.
 - Send it to the token queue for furthure processing.
*/
class Tokenizer
{
public:
	Tokenizer(Core &core);
	~Tokenizer(void);	
	// create a new token and send it to the token queue
	void wrapAndSend(vector<tuple<int*,short>> &distList, int &res, Context &cx);

	/*	fields	*/
	Core core;
};

