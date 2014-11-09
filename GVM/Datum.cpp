/** 
* Datum.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Datum.h"


Datum::~Datum(void)
{
}

Datum::Datum(int iValue,float fValue, char cValue,const string& sValue) {
	this->iValue = iValue;
	this->fValue = fValue;
	this->cValue = cValue;
	this->sValue = sValue;
}

//changing the type
void Datum::setTo(Type eType)
{
	// TODO
	this->iValue = this->bValue;
}

// overloading primitive operators
/*	==	*/
Datum Datum::operator==(const Datum& b)
{
	Datum retrn;
	// setting the token type for the new created Datum
	retrn.token_Type = this->token_Type;
	// silly way for checking the datum type and act accordingly
	if(this->token_Type == INT)
		retrn.bValue = this->iValue == b.iValue; 
	else if(this->token_Type == FLOAT)
		retrn.bValue = this->fValue == b.fValue; 

	return retrn;
}

// overloading primitive operators
/*	+	*/
Datum Datum::operator+(const Datum& b)
{
	Datum retrn;
	// setting the token type for the new created Datum
	retrn.token_Type = this->token_Type;
	// silly way for checking the datum type and act accordingly
	if(this->token_Type == INT)
		retrn.iValue = this->iValue + b.iValue; 
	else if(this->token_Type == FLOAT)
		retrn.fValue = this->fValue + b.fValue; 

	return retrn;
}
/*	-	*/
Datum Datum::operator-(const Datum& b)
{
	Datum retrn;
	// setting the token type for the new created Datum
	retrn.token_Type = this->token_Type;
	// silly way for checking the datum type and act accordingly
	if(this->token_Type == INT)
		retrn.iValue = this->iValue - b.iValue; 
	else if(this->token_Type == FLOAT)
		retrn.fValue = this->fValue - b.fValue; 

	return retrn;
}
/*	*	*/
Datum Datum::operator*(const Datum& b)
{
	Datum retrn;
	// setting the token type for the new created Datum
	retrn.token_Type = this->token_Type;
	// silly way for checking the datum type and act accordingly
	if(this->token_Type == INT)
		retrn.iValue = this->iValue * b.iValue; 
	else if(this->token_Type == FLOAT)
		retrn.fValue = this->fValue * b.fValue; 

	return retrn;
}
/*	/	*/
Datum Datum::operator/(const Datum& b)
{
	Datum retrn;
	// setting the token type for the new created Datum
	retrn.token_Type = this->token_Type;
	// silly way for checking the datum type and act accordingly
	if(this->token_Type == INT)
		retrn.iValue = this->iValue / b.iValue; 
	else if(this->token_Type == FLOAT)
		retrn.fValue = this->fValue / b.fValue; 

	return retrn;
}
/*	>	*/
Datum Datum::operator>(const Datum& b)
{
	Datum retrn;
	// setting the token type for the new created Datum
	retrn.token_Type = BOOLEAN;
	// silly way for checking the datum type and act accordingly
	if(this->token_Type == INT)
		retrn.bValue = this->iValue > b.iValue; 
	else if(this->token_Type == FLOAT)
		retrn.bValue = this->fValue > b.fValue; 

	return retrn;
}
/*	<	*/
Datum Datum::operator<(const Datum& b)
{
	Datum retrn;
	// setting the token type for the new created Datum
	retrn.token_Type = BOOLEAN;
	// silly way for checking the datum type and act accordingly
	if(this->token_Type == INT)
		retrn.bValue = this->iValue < b.iValue; 
	else if(this->token_Type == FLOAT)
		retrn.bValue = this->fValue < b.fValue; 

	return retrn;
}

/*
	This function acts as an abstraction level for the split
	to use it without worrying about the supported data types

vector<Datum> Datum::returnVectorOfDatums(Datum &dat)
{
	
}*/