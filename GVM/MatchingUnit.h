
/** 
* MatchingUnit.h
* Amr Gamil
* GVM
*/

#pragma once
#include <map>
#include "Core.h"
#include "Token.h"

using namespace std;

/*
* This class encapsulate the functionality of the matching unit.
* The matching unit is resposible for:
	- Recieving new tokens from the TokenDispatcher unit
	- Try to match those token to the existing tokens in the token table
	- If there is a hit, construct an executable packet and sends it to
	  the schedualer.
	- Otherwise, just update the local table with the recieved token
*/
class MatchingUnit
{
public:
	MatchingUnit();
	~MatchingUnit(void);

	void executeOrUpdateTable(Token<int> *tok);

	//fields

	// the core that this matching unit is associated with
	Core core;
	// The local tokens table that reflects the existing tokens
	// in the matching units that are waiting for thier pairs
	map<pair<long, long>, Token<int>*> tokenTable;
};

