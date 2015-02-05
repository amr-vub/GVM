/** 
* Tokenizer.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Tokenizer.h"
#include "Core.h"
#include "Instruction.h"
#include <limits>

vector<Core*> Tokenizer::coreList;

// helper function to get the MAX loaded neighbor core id
short myMax(short list[NIEGHBOORS], Core *core);

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
void Tokenizer::wrapAndSend(Vector_Tuple &distList, Datum &res, Context &cx, short &coreId, short token_executor_coreID)
{
	// loop through destination list, for each, create a token and send it to the token queue
	for (Vector_Tuple::iterator it = distList.begin(); it != distList.end(); ++it)
	{
		int* instAdd = get<0>(*it);
		short port = get<1>(*it);

		// construct the new Token
		Tag *newTag = new Tag(cx, port, instAdd, token_executor_coreID);
		Token_Type* newTok = new Token_Type(res, newTag);				

		// check if the new instruction is also independant
		Instruction *inst = this->core->memory.get(instAdd);

		if(instAdd[0] == 0 && instAdd[1] == 0)
			instAdd[0] = 0;

		// Send to the core's queue 
		if(coreId != token_executor_coreID)
		{
			// this was a stolen token
			if(inst->isINDependant())
				// then we can also continue exectuting the stolen token
				this->coreList[coreId]->insertToken_InIndInbox(newTok);	
			else
				// else return it to it's owner
				this->coreList[token_executor_coreID]->insertToken(newTok);		
		}
		else // that is not a stolen token
		{
			if(inst->isINDependant())
			{
				this->coreList[coreId]->insertToken_InIndInbox(newTok);		
			}
			else
			{
				this->coreList[coreId]->insertToken(newTok);		
			}
		}
	}	
}

// The load distrubuter handler function - functional level parallelism (new context --> new core)
// Here the overloaded core initiate the load distrubution
short Tokenizer::loadDistrubuter()
{
	int MIN = numeric_limits<int>::max();
	short coreID=this->core->coreID;
	// initially, we will use a naive load distrubuter technique based on the min size of core's queue
	for(auto cr : coreList)
	{
		if(cr->inbox.size() + cr->ind_Inbox.size()<MIN)
		{
			MIN = cr->inbox.size() + cr->ind_Inbox.size();
			coreID = cr->coreID;
		}
	}
	// corID now has the id for the selected core
	//printf("% \n",coreID);
	return coreID;
}

// The load balanacer handler function
// This is different than the load distrubuter function,
// as the underloaded core steals work from direct neighbor overloaded cores.
void Tokenizer::loadBalancer()
{
	if(this->core->coreID == 0)
		return;
	// first get the most overloaded core from the neighbors
	short victim = myMax(this->core->nieghborList, this->core);

	int stolenTokensCount= 0;
	// if there exist a core with load value over the THRESHOLD
	if(victim != -1)
	{
		boost::lock_guard<boost::mutex> guard(this->coreList[victim]->ind_c_mutex);	
		if(this->coreList[victim]->ind_Inbox.size() < THRESHOLD)
			return;
		//int victim_InboxSize = this->coreList[victim]->ind_Inbox.size();	
		const int victim_InboxSize_const = this->coreList[victim]->ind_Inbox.size();				
		// begin work stealing!!
		// loop through the victim's inbox, and steal half of it
		/**/									
		for (list<Token_Type*>::iterator it = this->coreList[victim]->ind_Inbox.begin();
			it!=this->coreList[victim]->ind_Inbox.end();++it)
		{
			// (2)							
			Token_Type *stolenToken = (*it);									
			this->core->insertToken_InIndInbox(stolenToken);		
			it = this->coreList[victim]->ind_Inbox.erase(it);
			//it = list<Token_Type*>::reverse_iterator();
			stolenTokensCount++;
			this->core->Idle_Counter++;
			/*
			cout<< "Token is stolen from core: " << this->coreList[victim]->coreID << 
				" to core: " << this->core->coreID << " and Idle counter is: " << this->core->Idle_Counter<< endl;
			*/	
			if(victim_InboxSize_const / 2 <= stolenTokensCount)
				break;		
			if(it == this->coreList[victim]->ind_Inbox.end())
				break;
			//victim_InboxSize = this->coreList[victim]->ind_Inbox.size();
		}
		
	}
	//cout<< ""<<endl;
}
// helper function to get the MAX loaded neighbor core id
short myMax(short list[NIEGHBOORS], Core *core)
{
	short MAX = THRESHOLD;
	short victim = -1;
	for (int i = 0; i < NIEGHBOORS; i++)
	{
		short coreIDx = core->nieghborList[i];
		int inbxSize = core->tokenizer.coreList[coreIDx]->ind_Inbox.size();
		if(core->tokenizer.coreList[coreIDx]->ind_Inbox.size() > MAX)
		{
			MAX = inbxSize;
			victim = coreIDx;
		}
	}
	return victim;
}

// propogate the stop token
void Tokenizer::sendStop(Token_Type *tok)
{
	this->core->insertToken(tok);
	// deactivate all of the other cores
	this->core->active = false;
	/*
	for(auto cr: coreList)
		cr->active = false;
		*/
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
		short crID = this->tokenizer->core->coreID;
		this->tokenizer->wrapAndSend(dest, (*it)->data, (*it)->tag->conx, crID, crID);
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

			// bind and send the recieved tok
			short newCoreId = bind_send(tok, destAdd, -1, tok.tag->port, retAdd, rest, new_cx, -1);

			// update the context map, cause we have still binds-1 toks to come
			contextMapContent csMapContent = {new_cx, binds-1, newCoreId};
			this->contextMap[tok.tag->tokenID] = csMapContent;	

		}
		else
		{
			// then no need to create a new context, just get the already created on
			// for the same old context Id
			new_cx = this->contextMap[tok.tag->tokenID].cx;
			bds = this->contextMap[tok.tag->tokenID].bds;
			short oldCoreId = this->contextMap[tok.tag->tokenID].coreId;
			// check if the stored binds is == 1. i.e. all of the expected toks have arrived
			if(bds == 1)
				// delete the entry from the context map
				this->contextMap.erase(tok.tag->tokenID);
			else
			{
				// update the context map, cause we have still binds-1 toks to come
				//contextMapContent csMapContent = {new_cx, bds-1, oldCoreId};
				this->contextMap[tok.tag->tokenID].bds--;	
			}
			// bind and send the recieved tok
			bind_send(tok, destAdd, -1, tok.tag->port, retAdd, rest, new_cx, oldCoreId);
		}
	}
	else
	{
		// generate new context
		new_cx = this->tokenizer->core->conxObj.getUniqueCx(this->tokenizer->core->coreID);
		// just changing the cx of one token, no further to come
		bind_send(tok, destAdd, tok.tag->port, tok.tag->port, retAdd, rest, new_cx, -1);
	}
}

/* 
	In case of single bind. <binds> == 1
	- Chenge the cx of the recieved token
	- Save the old cx together with the retAdd, chunk, port & restores
	- Delegate it to the tokenizer
*/
short ContextManager::bind_send(Token_Type &tok, int* destAdd, short destPort, short sentPort,
							   int* retAdd, short rest, Context* new_cx, short corId)
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
	if(corId == -1)
	{
		short newCorId = this->tokenizer->loadDistrubuter();
		this->tokenizer->wrapAndSend(temp ,tok.data, *new_cx, newCorId, newCorId);
		return newCorId;
	}
	
	this->tokenizer->wrapAndSend(temp ,tok.data, *new_cx, corId, corId);

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
	short cxCoreId = tok.tag->conx.coreId;
	if(cxCoreId == crID)
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

		short exe_CorId = tok.tag->token_executor_coreID;
		this->tokenizer->wrapAndSend(temp, tok.data, old_cx,crID, exe_CorId);

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
		Vector_Tuple tempDest;
		tempDest.push_back(make_tuple(tok.tag->instAdd, tok.tag->port));
		this->tokenizer->wrapAndSend(tempDest, tok.data, tok.tag->conx, cxCoreId, cxCoreId);
		/*
		Tag *newTag = new Tag(tok.tag->conx, tok.tag->port, tok.tag->instAdd, cxCoreId);		
		Token_Type* external_token = new Token_Type(tok.data, newTag);
		this->tokenizer->coreList[cxCoreId]->insertToken(external_token);
		*/
	}
}