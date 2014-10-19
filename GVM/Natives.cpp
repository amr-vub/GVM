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
int Natives::addNative(int v1, int v2)
{
	return v1 + v2;
}

// static function to intialize the map 
map<string, MyFuncPtrType> Natives::generateMap()
{
	map<string, int(*)(int,int)> temp;
	temp["add"] = &Natives::addNative;

	return temp;
}

// populate the map with function pointers to the native ops
map<string, MyFuncPtrType> Natives::opcodes_pointers = Natives::generateMap();