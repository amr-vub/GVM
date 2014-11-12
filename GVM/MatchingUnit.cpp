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
	// get the token context
	Context cx = tok->tag->conx;

	// get the token dist instruction address
	long instIdx = tok->tag->instIdx;

	// query the local table to see if a token already exists
	// TODO : Replace Make pair (or not?)
	TokenTableType::iterator tokenIt = 
		tokenTable.find(make_pair(cx.conxId, instIdx));

	if(tokenIt != tokenTable.end()){
		// there is a match for the recieved token, then
		// check if thier no more inputs to come
		vector<Token_Type*> tokens = get<1>(tokenIt->second);
		tokens.push_back(tok);
		// get<0> -> input size ; get<1> -> vector of tokens
		if(get<0>(tokenIt->second) == tokens.size())
		{
			// fetch and send them to the schedualer
			core->sch.executeTwo(tokens);
			tokenTable.erase(tokenIt);		
		}
		else
		{
			// still more inputs to come , i.e only in the case of array operation,
			// then update the tokentable
			tokenTable[make_pair(cx.conxId, instIdx)] = make_tuple(get<0>(tokenIt->second),tokens);
		}
	}
	else
	{
		// check first if the dest inst has literals
		Operation* inst = (Operation*) IMemory::get(tok->tag->instAdd);
		vector<Token_Type*> tempV = vector<Token_Type*>();
		tempV.push_back(tok);
		if(inst->tokenInputs == inst->inputs && inst->inputs == 1)
		{
			//send it to the sch
			this->core->sch.executeTwo(tempV);
			delete tok;
		}
		else if(inst->literals.empty())
		{
			// save the token in the token table, and wait for it's pair
			tokenTable[make_pair(cx.conxId, instIdx)] = make_tuple(inst->tokenInputs, tempV);			
		}
		else
		{
			// prepare the literal as a token
			tuple<short, Datum> temp = inst->literals.front();
			short port = get<0>(temp);
			Datum value = get<1>(temp);
			Tag *tag = new Tag(tok->tag->conx, port, tok->tag->instAdd);
			Token_Type *tok2 = new Token_Type(value, tag);
			if(port < tok->tag->port)
				tempV.insert(tempV.begin(), tok2);
			else
				tempV.insert(tempV.end(), tok2);
			//send it to the sch
			this->core->sch.executeTwo(tempV);
			//tempV.clear();
			// delete here
			delete tok2;	
			delete tok;
		}
	}
}