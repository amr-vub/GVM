/** 
* Tokenizer.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Tokenizer.h"

Tokenizer::Tokenizer(Core &core)
{
	this->core = core;
	this->swicther = new Switcher(*this);
}

Tokenizer::Tokenizer(void)
{
}

Tokenizer::~Tokenizer(void)
{
	delete this->swicther;
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
void Tokenizer::wrapAndSend(Tuple_vector &distList, int &res, Context &cx)
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


Switcher::Switcher()
{
}

Switcher::Switcher(Tokenizer &tokenizer)
{
}


Switcher::~Switcher()
{
}
// add an element to the map
void Switcher::addSwitchStorageElement(Token<int> &tok)
{
	long cx = tok.tag.conx.conxId;	
	this->switchStorage[cx].push_back(tok);
}

//get an element from the storage based on the context
Vector_token Switcher::getAllElement(long &cx)
{
	Vector_token temp = this->switchStorage[cx];
	this->switchStorage.erase(cx);
	return temp;
}

/*
	for the recieved dest and token vectors, loop through the the tokens and for each
	delegate to the tokenizer to send them to the queue
*/
void Switcher::sendToTokinzer(Tuple_vector &dest,Vector_token &tokV)
{
	for(Vector_token::iterator it = tokV.begin(); it!=tokV.end(); ++it)
	{
		this->tokenizer.wrapAndSend(dest, (*it).data, (*it).tag.conx);
	}
}