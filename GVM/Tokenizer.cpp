/** 
* Tokenizer.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include <tuple>
#include "Tokenizer.h"
#include "Token.h"


typedef vector<tuple<int*,short> > Tuple_vector;

Tokenizer::Tokenizer(Core &core)
{
	this->core = core;
}


Tokenizer::~Tokenizer(void)
{
}
/*
* wrapAndSend Function
* This function create a new token with the input res as the data element, cx as the context
* and send copies of it for each inst in the destination list
	\param:
		distList: vector of tuples (instAddress Array, port number)
	\param:
		res: the result of the executed computation
	\param:
		cx: new token context
*/
void Tokenizer::wrapAndSend(vector<tuple<int*,short>> &distList, int &res, Context &cx)
{
	// loop through destination list, for each, create a token and send it to the token queue
	for (Tuple_vector::iterator it = distList.begin(); it != distList.end(); ++it)
	{
		int* instAdd = get<0>(*it);
		short port = get<1>(*it);

		// construct the new Token
		Tag newTag = Tag(cx, port, instAdd);
		Token<int> newTok = Token<int>(res, newTag);
		
		// TODO, send to the queue
		//this->core.inbox.push_back(newTok);
	}	
}