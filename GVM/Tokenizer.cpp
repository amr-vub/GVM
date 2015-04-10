/** 
* Tokenizer.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Tokenizer.h"
#include "Core.h"

Tokenizer::Tokenizer() 
{
	this->swicther =  Switcher();
	this->contextManager = ContextManager();

	this->swicther.tokenizer = this;
	contextManager.tokenizer = this;
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
void Tokenizer::wrapAndSend(Vector_Tuple &distList, Datum &res, Context &cx)
{
	// loop through destination list, for each, create a token and send it to the token queue
	for (Vector_Tuple::iterator it = distList.begin(); it != distList.end(); ++it)
	{
		int* instAdd = get<0>(*it);
		short port = get<1>(*it);

		// construct the new Token
		Tag *newTag = new Tag(cx, port, instAdd);
		Token_Type* newTok = new Token_Type(res, newTag);
		
		LOG("Wrapping Token with data type " + to_string(res.token_Type) + " and data value "
			+ to_string(res.iValue) + " context " + to_string(cx.conxId) + " and instruction address ["
			+  to_string(instAdd[0]) + "," + to_string(instAdd[1])+ "] \n");

		// TODO, send to the queue
		this->core->inbox.push_back(newTok);
	}	
}

// 
void Tokenizer::sendStop(Token_Type *tok)
{
	this->core->inbox.push_back(tok);
	this->core->active = false;
}

Switcher::Switcher()
{
}


Switcher::~Switcher()
{
}
// save the dest add and number of remaining inputs for the given (cx,insAdd) combination
void Switcher::storeDest(Token_Type *tok, short remainingInputs, int* indx)
{
	long cx = tok->tag->conx.conxId;	
	long instAdd = tok->tag->instIdx;
	int* destAdd = new int[2];
	destAdd[0] = indx[0];
	destAdd[1] = indx[1];
	this->savedDestinations[make_pair(cx, instAdd)] = make_tuple(remainingInputs, destAdd);
}

// update and do garbage collection if needed in the savedDestinations
tuple<short, int*>  Switcher::updateStoredDest(Token_Type *tok)
{
	long cx = tok->tag->conx.conxId;	
	long instAdd = tok->tag->instIdx;
	tuple<short, int*> temp = this->savedDestinations[make_pair(cx, instAdd)];
	short remaining_inputs = get<0>(temp) - 1;
	if(remaining_inputs <= 0)
		this->savedDestinations.erase(make_pair(cx, instAdd));
	else
		this->savedDestinations[make_pair(cx, instAdd)] = make_tuple(remaining_inputs, get<1>(temp));
	return temp;
}
// checks if there is a stored dest for this token
bool Switcher::alreadyExists(Token_Type *tok)
{
	long cx = tok->tag->conx.conxId;	
	long instAdd = tok->tag->instIdx;
	if(this->savedDestinations.find(make_pair(cx, instAdd)) != this->savedDestinations.end())
		return true;
	return false;
}

// add an element to the map
void Switcher::addSwitchStorageElement(Token_Type* tok)
{
	long cx = tok->tag->conx.conxId;	
	long instAdd = tok->tag->instIdx;
	this->switchStorage[make_pair(cx, instAdd)].push_back(tok);
}

//get an element from the storage based on the context
Vector_token Switcher::getAllElement(long &cx, long &instAdd)
{
	Vector_token temp = this->switchStorage[make_pair(cx, instAdd)];
	this->switchStorage.erase(make_pair(cx, instAdd));
	return temp;
}

/*
	for the recieved dest and token vectors, loop through the the tokens and for each
	delegate to the tokenizer to send them to the queue
*/
void Switcher::sendToTokinzer(Vector_Tuple &dest,Vector_token &tokV)
{
	for(Vector_token::iterator it = tokV.begin(); it!=tokV.end(); ++it)
	{
		this->tokenizer->wrapAndSend(dest, (*it)->data, (*it)->tag->conx);
		// freeing memory							
		delete *it;
	}
	tokV.clear();
}

/*	ContextManager	*/
ContextManager::ContextManager()
{
}


ContextManager::~ContextManager()
{
}
/*	
	This function handels the context change in case of multiple binds i.e. <binds> > 1
	
	\param: tok
		the recieved token to change it's context
	\param: retAdd
		The add to return to when we context is restored
	\param: binds
		how many argument I expect to attach the same context to
	\param: rest
		how many return arguments I expect from this call	
*/
void ContextManager::bind_save(Token_Type &tok, int* destAdd, int* retAdd, short &binds, short rest, 
							   long &instIdx, vector<tuple<short, Datum>> literals)
{
	Context old_cx = tok.tag->conx;
	Context *new_cx = NULL;
	short bds;
	LOG("ContextManager::bind_save -- Token with context " + to_string(old_cx.conxId) + " --");
	// check if we need to store the new generated cx for other incoming toks
	if(binds > 1)
	{	
		LOG(" Bind > 1 --");
		// if no element already exists for that tok. i.e the first argument to arrive
		if(this->contextMap.find(make_pair(old_cx.conxId, instIdx)) == contextMap.end())
		{
			LOG(" Generate new context -- \n");
			// generate new context
			new_cx = this->tokenizer->core->conxObj.getUniqueCx(this->tokenizer->core->coreID);

			// means that there exist at least one literal, then compare the binds to the literals size
			if(!literals.empty())
				for(auto lit : literals)
				{
					// prepare the literal as a token			
					short port = get<0>(lit);
					Datum value = get<1>(lit);
					Tag *tag = new Tag(tok.tag->conx, port, tok.tag->instAdd);
					Token_Type tok2 =  Token_Type(value, tag);
					// then send delegate
					bind_send(tok2, destAdd, -1, port, retAdd, rest, new_cx);
					LOG("Sending literak with port : [" + to_string(port)+ "] \n");
				}

			// update the context map, cause we have still binds-1 toks to come
			this->contextMap[make_pair(old_cx.conxId, instIdx)] = make_tuple(binds-1, new_cx);			

		}
		else
		{
			LOG(" NO need to create a new context --");
			// then no need to create a new context, just get the already created on
			// for the same old context Id
			new_cx = get<1>(this->contextMap[make_pair(old_cx.conxId, instIdx)]);
			bds = get<0>(this->contextMap[make_pair(old_cx.conxId, instIdx)]);
			// check if the stored binds is == 1. i.e. all of the expected toks have arrived
			if(bds == 1)
			{
				LOG(" Delete the entry from the context map -- \n");
				// delete the entry from the context map
				this->contextMap.erase(make_pair(old_cx.conxId, instIdx));
			}
			else
			{
				LOG(" Still have tokens to come: " + to_string(bds-1) + " -- \n");
				// update the context map, cause we have still binds-1 toks to come
				this->contextMap[make_pair(old_cx.conxId, instIdx)] = make_tuple(bds-1, new_cx);	
			}
		}
		// bind and send the recieved tok
		bind_send(tok, destAdd, -1, tok.tag->port, retAdd, rest, new_cx);
	}
	else
	{
		LOG(" ELSE PART: Generate new context -- \n");
		// generate new context
		new_cx = this->tokenizer->core->conxObj.getUniqueCx(this->tokenizer->core->coreID);
		// just changing the cx of one token, no further to come
		bind_send(tok, destAdd, -1, tok.tag->port, retAdd, rest, new_cx);
	}
}

/* 
	In case of single bind. <binds> == 1
	- Chenge the cx of the recieved token
	- Save the old cx together with the retAdd, chunk, port & restores
	- Delegate it to the tokenizer
*/
void ContextManager::bind_send(Token_Type &tok, int* destAdd, short destPort, short sentPort,
							   int* retAdd, short rest, Context* new_cx)
{
	Context *old_cx = new Context();
	*old_cx = tok.tag->conx;
	// store all of the req info to restore the cx in the restore map
	RestoreArgs restArgs =  
	{
		retAdd[0],	// det chunk 	
		retAdd[1],	// dest instruction address
		destPort,// dest port number
		old_cx, 	// old context	
		rest,		// number of expected return values
	};
	restoreMap[new_cx->conxId] = restArgs;	
	
	// send the tok to the tokenizer
	Vector_Tuple temp;
	temp.push_back(make_tuple(destAdd, sentPort));
	this->tokenizer->wrapAndSend(temp ,tok.data, *new_cx);
}

/*
	Restore the cx for the recieved tok
	\param: tok
		the recieved token to restore it's cx
*/
void ContextManager::restore(Token_Type &tok)
{
	// query the restore map by the  to get the resArgs
	/*
		struct RestoreArgs{	
			short chunk;
			int idx;
			short port;
			Context *cx;
			short restores;
		};
	*/	
	long conxId = tok.tag->conx.conxId;
	RestoreArgs resArgs = restoreMap[conxId];
	Context old_cx = *resArgs.cx;

	LOG("RESTORING: token with cx: " + to_string(conxId) + " and old_cx: " + to_string(old_cx.conxId) + 
		" Dest inst: [" + to_string(resArgs.chunk) + "," + to_string(resArgs.idx) + "] \n");

	// send the tok to the tokenizer
	Vector_Tuple temp;
	int index[2] = {resArgs.chunk, resArgs.idx};
	// chekc if we need to change the port or not
	if(resArgs.port != -1)
		temp.push_back(make_tuple(index, resArgs.port));
	else if(resArgs.port == -1)
		temp.push_back(make_tuple(index, tok.tag->port));

	this->tokenizer->wrapAndSend(temp, tok.data, old_cx);

	// if restore value is one, then we no longer need to save this entry
	if(resArgs.restores <= 1)
	{
		Context *cx = new Context();
		*cx = tok.tag->conx;
		// append the cx to the avaliable cx pool
		this->tokenizer->core->conxObj.freeContext(cx);
		// erase the entry in the restore map
		restoreMap.erase(conxId);
	}
	else
	{
		resArgs.restores--;
		restoreMap[conxId] = resArgs;
	}
}