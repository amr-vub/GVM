/** 
* Natives.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Natives.h"


Natives::Natives(void)
{
}


Natives::~Natives(void)
{
}

// Add 2 arguments and returns the output
Datum Natives::addNative(vector<Datum> args)
{
	return args[0] + args[1];
}

// Sub 2 arguments and returns the output
Datum Natives::subNative(vector<Datum> args)
{
	return args[0] - args[1];
}


// Sub 2 arguments and returns the output
Datum Natives::mulNative(vector<Datum> args)
{
	return args[0] * args[1];
}

// Return true if thefirst input is greater than right input.
Datum Natives::moreNative(vector<Datum> args)
{
	return args[0] > args[1];
}

// Return true if thefirst input is smaller than right input.
Datum Natives::lessNative(vector<Datum> args)
{
	return args[0] < args[1];
}

// Return the input as an int
Datum Natives::intNative(vector<Datum> args)
{
	// here we should check the original type for the input datum
	if(args[0].token_Type == Datum::FLOAT)
	{
		args[0].iValue = args[0].fValue;		
	}
	else if(args[0].token_Type == Datum::BOOLEAN)
	{
		args[0].iValue = args[0].bValue;
		
	}
	// TODO; else parts

	args[0].token_Type = Datum::INT;

	return args[0];
}

// Return the true if both inputs are true
Datum Natives::equalsNative(vector<Datum> args)
{
	return args[0] == args[1];
}


// returns an vector with range specified in the datum of the inputs
Datum Natives::rangeNative(vector<Datum> args)
{
	Datum ret;
	// set the tokendata type
	ret.token_Type = Datum::I_VECTOR;	
	//generate the range
	for(int i=args[0].iValue; i<=args[1].iValue;i++)
		ret.iValue_v.push_back(i);

	return ret;
}


// takess two vectors, and returns one vector of both of them concatenated
Datum Natives::vecCatNative(vector<Datum> args)
{		
	// set the tokendata type
	if(args[0].token_Type == Datum::I_VECTOR)
	{			
		//make the contatenation
		for(vector<int>::iterator it = args[1].iValue_v.begin() ; it != args[1].iValue_v.end(); ++it)
			args[0].iValue_v.push_back(*it);
	}
	else if(args[0].token_Type == Datum::F_VECTOR)
	{
		for(vector<float>::iterator it = args[1].fValue_v.begin() ; it != args[1].fValue_v.end(); ++it)
			args[0].fValue_v.push_back(*it);
	}

	return args[0];
}

/*
	Takess two vectors. The first is values, the second is booleans.
	If the boolean value is true, the crossponding value in the
	first array is kept, otherwise it's pruned
*/
Datum Natives::vecPruneNative(vector<Datum> args)
{		
	Datum ret;
	ret.token_Type = args[0].token_Type;
	int idx = 0;
	// set the tokendata type
	if(args[0].token_Type == Datum::I_VECTOR)
	{			
		//make the pruning
		
		for(vector<int>::iterator it = args[1].iValue_v.begin() ; it != args[1].iValue_v.end(); ++it)
			if(*it)
				ret.iValue_v.push_back(args[0].iValue_v[idx++]);		
	}
	else if(args[0].token_Type == Datum::F_VECTOR)
	{
		idx = 0;
		for(vector<float>::iterator it = args[1].fValue_v.begin() ; it != args[1].fValue_v.end(); ++it)
			if(*it)
				ret.fValue_v.push_back(args[0].fValue_v[idx++]);
	}

	return ret;
}

// Returns an vector from the input tokens
Datum Natives::vecNative(vector<Datum> args)
{
	Datum ret;		
	if(args[0].token_Type == Datum::INT)
	{
		ret.token_Type = Datum::I_VECTOR;
		for (int i = 1; i < args.size(); i++)
		{
			ret.iValue_v.push_back(args[i].iValue);
		}
	}
	else if(args[0].token_Type == Datum::FLOAT)
	{
		ret.token_Type = Datum::F_VECTOR;
		for (int i = 1; i < args.size(); i++)
		{
			ret.iValue_v.push_back(args[i].fValue);
		}
	}
	
	return ret;
}

// static function to intialize the map 
map<string, MyFuncPtrType> Natives::generateMap()
{
	map<string, MyFuncPtrType> temp;
	temp["add"] = &Natives::addNative;
	temp["sub"] = &Natives::subNative;
	temp["mul"] = &Natives::mulNative;
	temp["more"] = &Natives::moreNative;
	temp["less"] = &Natives::lessNative;
	temp["int"] = &Natives::intNative;
	temp["range"] = &Natives::rangeNative;
	temp["arrCat"] = &Natives::vecCatNative;
	temp["arrPrune"] = &Natives::vecPruneNative;
	temp["equal"] = &Natives::equalsNative;
	temp["array"] = &Natives::vecNative;

	return temp;
}

// populate the map with function pointers to the native ops
map<string, MyFuncPtrType> Natives::opcodes_pointers = Natives::generateMap();