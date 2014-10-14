
/** 
* MatchingUnit.h
* Amr Gamil
* GVM
*/

#pragma once

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
	MatchingUnit(void);
	~MatchingUnit(void);
};

