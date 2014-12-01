/** 
* Tokenizer.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Tokenizer.h"
#include "Core.h"
#include <limits>

vector<Core*> Tokenizer::coreList;

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
void Tokenizer::wrapAndSend(Vector_Tuple &distList, Datum &res, Context &cx, short &coreId)
{
	// loop through destination list, for each, create a token and send it to the token queue
	for (Vector_Tuple::iterator it = distList.begin(); it != distList.end(); ++it)
	{
		int* instAdd = get<0>(*it);
		short port = get<1>(*it);

		// construct the new Token
		Tag *newTag = new Tag(cx, port, instAdd);
		Token_Type* newTok = new Token_Type(res, newTag);
		
		// Send to the core's queue 
		this->coreList[coreId]->insertToken(newTok);		
	}	
}

// The load balanacer handler function
short Tokenizer::loadBalancer()
{
	int MIN = numeric_limits<int>::max();
	short coreID=0;
	// initially, we will use a naive load balancing technique based on the min size of core's queue
	for(auto cr : coreList)
	{
		if(cr->inbox.size()<MIN)
		{
			MIN = cr->inbox.size();
			coreID = cr->coreID;
		}
	}
	// corID now has the id for the selected core
	//printf("% \n",coreID);
	return coreID;
}

// propogate the stop token
void Tokenizer::sendStop(Token_Type *tok)
{
	this->core->insertToken(tok);
	// deactivate all of the other cores
	for(auto cr: coreList)
		cr->active = false;
}

Switcher::Switcher()
{
}


Switcher::~Switcher()
{
}
// add an element to the map
void Switcher::addSwitchStorageElement(Token_Type* tok)
{
	this->switchStorage[tok->tag->tokenID].push_back(tok);
}

//get an element from the storage based on the context
Vector_token Switcher::getAllElement(unsigned long long &tokenId)
{
	Vector_token temp = this->switchStorage[tokenId];
	this->switchStorage.erase(tokenId);
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
		this->tokenizer->wrapAndSend(dest, (*it)->data, (*it)->tag->conx, this->tokenizer->core->coreID);
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
void ContextManager::bind_save(Token_Type &tok, int* destAdd, int* retAdd, short &binds, short rest, unsigned long &instIdx)
{
	Context old_cx = tok.tag->conx;
	Context *new_cx = NULL;
	short bds;
	// check if we need to store the new generated cx for other incoming toks
	if(binds > 1)
	{
		// if no element already exists for that tok. i.e the first argument to arrive
		if(this->contextMap.find(tok.tag->tokenID) == contextMap.end())
		{
			// generate new context
			new_cx = this->tokenizer->core->conxObj.getUniqueCx(this->tokenizer->core->coreID);				
			// update the context map, cause we have still binds-1 toks to come
			this->contextMap[tok.tag->tokenID] = make_tuple(binds-1, new_cx);			

		}
		else
		{
			// then no need to create a new context, just get the already created on
			// for the same old context Id
			new_cx = get<1>(this->contextMap[tok.tag->tokenID]);
			bds = get<0>(this->contextMap[tok.tag->tokenID]);
			// check if the stored binds is == 1. i.e. all of the expected toks have arrived
			if(bds == 1)
				// delete the entry from the context map
				this->contextMap.erase(tok.tag->tokenID);
			else
				// update the context map, cause we have still binds-1 toks to come
				this->contextMap[tok.tag->tokenID] = make_tuple(bds-1, new_cx);	
		}
		// bind and send the recieved tok
		bind_send(tok, destAdd, -1, tok.tag->port, retAdd, rest, new_cx);
	}
	else
	{
		// generate new context
		new_cx = this->tokenizer->core->conxObj.getUniqueCx(this->tokenizer->core->coreID);
		// just changing the cx of one token, no further to come
		bind_send(tok, destAdd, tok.tag->port, tok.tag->port, retAdd, rest, new_cx);
	}
}

/* 
	In case of single bind. <binds> == 1
	- Chenge the cx of the recieved token
	- Save the old cx together with the retAdd, chunk, port & restores
	- Delegate it to the tokenizer
*/
short ContextManager::bind_send(Token_Type &tok, int* destAdd, short destPort, short sentPort,
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
	// resolve where to send the token
	short corId = this->tokenizer->loadBalancer();
	this->tokenizer->wrapAndSend(temp ,tok.data, *new_cx, corId);
	return corId;
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
	// first check if the core id field in the context object matches this core
	short crID = this->tokenizer->core->coreID;
	if(tok.tag->conx.coreId == crID)
	{
		long conxId = tok.tag->conx.conxId;
		RestoreArgs resArgs = restoreMap[conxId];
		Context old_cx = *resArgs.cx;

		// send the tok to the tokenizer
		Vector_Tuple temp;
		int index[2] = {resArgs.chunk, resArgs.idx};
		// chekc if we need to change the port or not
		if(resArgs.port != -1)
			temp.push_back(make_tuple(index, resArgs.port));
		else if(resArgs.port == -1)
			temp.push_back(make_tuple(index, tok.tag->port));

		this->tokenizer->wrapAndSend(temp, tok.data, old_cx, crID);

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
	else 
	// means that this tken was sent to us from other core id, then 
	// just for now TODO, sent it back to it's original core queue
	{
		Tag *newTag = new Tag(tok.tag->conx, tok.tag->port, tok.tag->instAdd);		
		Token_Type* external_token = new Token_Type(tok.data, newTag);
		this->tokenizer->coreList[tok.tag->conx.coreId]->insertToken(external_token);
	}
}