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

// Return true if thefirst input is greater than or equal right input.
Datum Natives::moreEqNative(vector<Datum> args)
{
	return args[0] >= args[1];
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
	switch(args[0].token_Type) 
	{			
	case Datum::I_VECTOR:
		//make the contatenation
		for(vector<int>::iterator it = args[1].iValue_v.begin() ; it != args[1].iValue_v.end(); ++it)
			args[0].iValue_v.push_back(*it);
		break;
	case Datum::F_VECTOR:	
		for(vector<float>::iterator it = args[1].fValue_v.begin() ; it != args[1].fValue_v.end(); ++it)
			args[0].fValue_v.push_back(*it);	
		break;
	default:
		printf("_____ERORR_____ vecCatNative\n");
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
	// check the tokendata type and make the pruning
	switch(args[0].token_Type)
	{		
	case Datum::I_VECTOR:
		{
			for(vector<int>::iterator it = args[1].iValue_v.begin() ; it != args[1].iValue_v.end(); ++it)
				if(*it)
					ret.iValue_v.push_back(args[0].iValue_v[idx++]);
			// In case the first arg was longer, then loop
			for(int i = idx; i<args[0].iValue_v.size();++i)
				ret.iValue_v.push_back(args[0].iValue_v[idx]);

		}
		break;
	case Datum::F_VECTOR:	
		{
			idx = 0;
			for(vector<float>::iterator it = args[1].fValue_v.begin() ; it != args[1].fValue_v.end(); ++it)
				if(*it)
					ret.fValue_v.push_back(args[0].fValue_v[idx++]);
						// In case the first arg was longer, then loop
				for(int i = idx; i<args[0].iValue_v.size();++i)
					ret.fValue_v.push_back(args[0].fValue_v[idx]);
		}
		break;
	case Datum::B_VECTOR:	
		{
			idx = 0;
			for(vector<bool>::iterator it = args[1].bValue_v.begin() ; it != args[1].bValue_v.end(); ++it)
				if(*it)
					ret.bValue_v.push_back(args[0].bValue_v[idx++]);
						// In case the first arg was longer, then loop
				for(int i = idx; i<args[0].iValue_v.size();++i)
					ret.bValue_v.push_back(args[0].bValue_v[idx]);
		}
		break;
	default:
		printf("_____ERORR_____ vecPruneNative\n");
	}

	return ret;
}

// Returns an vector from the input tokens
Datum Natives::vecNative(vector<Datum> args)
{
	Datum ret;		
	switch(args[0].token_Type)
	{
	case Datum::INT:
		{
			ret.token_Type = Datum::I_VECTOR;
			for (int i = 0; i < args.size(); i++)			
				ret.iValue_v.push_back(args[i].iValue);			
		}
		break;
	case Datum::FLOAT:
		{
			ret.token_Type = Datum::F_VECTOR;
			for (int i = 0; i < args.size(); i++)			
				ret.iValue_v.push_back(args[i].fValue);			
		}
		break;
	case Datum::BOOLEAN:
		{
			ret.token_Type = Datum::B_VECTOR;
			for (int i = 0; i < args.size(); i++)			
				ret.iValue_v.push_back(args[i].bValue);			
		}
		break;
	default:
		printf("_____ERORR_____ vecNative\n");
	}
	return ret;
}


// Returns the upper bound of the array
Datum Natives::vecBoundNative(vector<Datum> args)
{
	Datum ret;
	ret.token_Type = Datum::INT;

	switch (args[0].token_Type)
	{
	case Datum::I_VECTOR:
		ret.iValue = args[0].iValue_v.size()-1;
		break;
	case Datum::F_VECTOR:
		ret.iValue = args[0].fValue_v.size()-1;
		break;
	case Datum::B_VECTOR:
		ret.iValue = args[0].bValue_v.size()-1;
		break;
	default:
		printf("_____ERORR_____ vecBoundNative \n");
	}

	return ret;	
}

/*
Returns the element at the given index
args[0] -> vector
args[1] -> index (we assume the index is INT without checking)
*/
Datum Natives::vecGetNative(vector<Datum> args)
{
	Datum ret;	

	switch (args[0].token_Type)
	{
	case Datum::I_VECTOR:
		{
			ret.token_Type = Datum::INT;
			ret.iValue = args[0].iValue_v[args[1].iValue];			
		}
		break;
	case Datum::F_VECTOR:
		{
			ret.token_Type = Datum::FLOAT;
			ret.iValue = args[0].fValue_v[args[1].iValue];			
		}
		break;
	case Datum::B_VECTOR:
		{
			ret.token_Type = Datum::BOOLEAN;
			ret.bValue = args[0].bValue_v[args[1].iValue];			
		}
		break;
	default:
		printf("_____ERORR_____ vecGetNative \n");
	}

	return ret;	
}

// Retruns the length of the array
Datum Natives::vecLengthNative(vector<Datum> args)
{
	Datum ret;
	ret.token_Type = Datum::INT;

	switch (args[0].token_Type)
	{
	case Datum::I_VECTOR:
		ret.iValue = args[0].iValue_v.size();
		break;
	case Datum::F_VECTOR:
		ret.iValue = args[0].fValue_v.size();
		break;
	case Datum::B_VECTOR:
		ret.iValue = args[0].bValue_v.size();
		break;
	default:
		{
			//ret.iValue = 1;
			printf("_____ERORR_____ vecLengthNative \n");
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
	temp["moreEq"] = &Natives::moreEqNative;
	temp["less"] = &Natives::lessNative;
	temp["int"] = &Natives::intNative;
	temp["range"] = &Natives::rangeNative;
	temp["arrCat"] = &Natives::vecCatNative;
	temp["arrPrune"] = &Natives::vecPruneNative;
	temp["equals"] = &Natives::equalsNative;
	temp["array"] = &Natives::vecNative;
	temp["arrLen"] = &Natives::vecLengthNative;
	temp["arrGet"] = &Natives::vecGetNative;
	temp["arrBound"] = &Natives::vecBoundNative;

	return temp;
}

// populate the map with function pointers to the native ops
map<string, MyFuncPtrType> Natives::opcodes_pointers = Natives::generateMap();