/** 
* MatchingUnit.cpp
* Amr Gamil
* GVM
*/
#include "stdafx.h"
#include "MatchingUnit.h"
#include "Token.h"

// constructor
MatchingUnit::MatchingUnit(void)
{
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
	int *instAdd = tok.tag.instAdd;
	// query the local table to see if a token already exists
	// TODO : Replace Make pair
	if(tokenTable.find(make_pair(cx, instAdd)) != tokenTable.end())
		// there is a match for the
		;
}