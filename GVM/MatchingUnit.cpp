/** 
* MatchingUnit.cpp
* Amr Gamil
* GVM
*/
#include "stdafx.h"
#include "MatchingUnit.h"
#include "Scheduler.h"
#include "IMemory.h"
#include "Core.h"
#include "Instruction.h"

// constructor
MatchingUnit::MatchingUnit()
{
	//this->core = core;
}

// destructor
MatchingUnit::~MatchingUnit(void)
{
}
/*
* This function recieves a token and then make a check in it's local table
* If the token has it's pair already exists in the table, both are sent to
* the schedualer for execurion. 
* Otherwise, it store the token till it's pair arrives
* \param
		tok : the recieved token
*/
void MatchingUnit::executeOrUpdateTable(Token_Type *tok)
{
	/*
	if(tok->tag->instAdd[1]==12)
		printf("ada");
		*/
	// get the token context
	Context cx = tok->tag->conx;

	// get the token dist instruction address
	long instIdx = tok->tag->instIdx;

	// query the local table to see if a token already exists
	// TODO : Replace Make pair (or not?)
	TokenTableType::iterator tokenIt = 
		tokenTable.find(tok->tag->tokenID);

	if(tokenIt != tokenTable.end()){
		// there is a match for the recieved token, then
		// check if thier no more inputs to come
					
		if(tokenIt->second.Indx + 1 == tokenIt->second.inputs)
		{
			// fetch and send them to the schedualer
			tokenIt->second.tokenArr[tok->tag->port] = tok;
			core->sch.executeTwo(tokenIt->second.tokenArr);
			//delete memory for the array of tokens
			delete [] tokenIt->second.tokenArr;
			tokenTable.erase(tokenIt);						
		}
		else
		{
			// still more inputs to come , i.e only in the case of array operation,
			// then update the tokentable
			tokenIt->second.Indx++;
			tokenIt->second.tokenArr[tok->tag->port] = tok;			
			tokenTable[tok->tag->tokenID] = tokenIt->second;
		}
	}
	else
	{
		// check first if the dest inst has literals
		Instruction* inst = IMemory::get(tok->tag->instAdd);		

		Token_Type** tokensArr;

		if(inst->tokenInputs == inst->inputs && inst->inputs == 1)
		{
			//send it to the sch			
			this->core->sch.executeTwo(&tok);
			delete tok;			
		}
		else if(inst->literals.empty())
		{
			// First time to get a token with this indx
			// save the token in the token table, and wait for the rest			
			tokensArr = new Token_Type*[inst->inputs];
			tokensArr[tok->tag->port] = tok;
			TokenTableValue savedTKstruct = {
				savedTKstruct.inputs = inst->inputs,
				savedTKstruct.Indx = 1,
				savedTKstruct.tokenArr = tokensArr
			};
			tokenTable[tok->tag->tokenID] =	savedTKstruct;
		}
		else
		{
			// prepare the literal as a token
			tuple<short, Datum> temp = inst->literals.front();
			short port = get<0>(temp);
			Datum value = get<1>(temp);
			Tag *tag = new Tag(tok->tag->conx, port, tok->tag->instAdd, tok->tag->token_executor_coreID);
			Token_Type *tok2 = new Token_Type(value, tag);
			
			Token_Type* tokenPacket[2];

			if(port < tok->tag->port)
			{
				tokenPacket[0] = tok2;
				tokenPacket[1] = tok;
			}
			else
			{
				tokenPacket[0] = tok;
				tokenPacket[1] = tok2;
			}
			//send it to the sch
			this->core->sch.executeTwo(tokenPacket);
			//tempV.clear();
			// delete here
			delete tok2;	
			delete tok;			
		}
	}
}