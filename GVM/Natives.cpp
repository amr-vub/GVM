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
int Natives::addNative(vector<int> args)
{
	return args[0] + args[1];
}

// Sub 2 arguments and returns the output
int Natives::subNative(vector<int> args)
{
	return args[0] - args[1];
}


// Sub 2 arguments and returns the output
int Natives::mulNative(vector<int> args)
{
	return args[0] * args[1];
}

// Return true if thefirst input is greater than right input.
int Natives::moreNative(vector<int> args)
{
	return args[0] > args[1];
}

// Return the input as an int
int Natives::intNative(vector<int> args)
{
	return args[0];
}

// static function to intialize the map 
map<string, MyFuncPtrType> Natives::generateMap()
{
	map<string, MyFuncPtrType> temp;
	temp["add"] = &Natives::addNative;
	temp["sub"] = &Natives::subNative;
	temp["mul"] = &Natives::mulNative;
	temp["more"] = &Natives::moreNative;
	temp["int"] = &Natives::intNative;

	return temp;
}

// populate the map with function pointers to the native ops
map<string, MyFuncPtrType> Natives::opcodes_pointers = Natives::generateMap();