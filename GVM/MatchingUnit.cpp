/** 
* MatchingUnit.cpp
* Amr Gamil
* GVM
*/
#include "stdafx.h"
#include "MatchingUnit.h"
#include "Scheduler.h"
//#include 

// constructor
MatchingUnit::MatchingUnit(Core &core)
{
	this->core = core;
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
void MatchingUnit::executeOrUpdateTable(Token<int> tok)
{
	// get the token context
	Context cx = tok.tag.conx;

	// get the token dist instruction address
	long instIdx = tok.tag.instIdx;

	// query the local table to see if a token already exists
	// TODO : Replace Make pair (or not?)
	map<pair<long, long>, Token<int>>::iterator tokenIt = 
		tokenTable.find(make_pair(cx.conxId, instIdx));
	if(tokenIt != tokenTable.end()){
		// there is a match for the recieved token, then
		// fetch both and send them to the schedualer
		Token<int> tokens[2] = {tokenIt->second, tok};
		core.sch->execute(tokens);
		tokenTable.erase(tokenIt);		
	}
	else
	{
		// save the token in the token table, and wait for it's pair
		tokenTable[make_pair(cx.conxId, instIdx)] = tok;
	}
}