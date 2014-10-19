/** 
* Natives.h
* Amr Gamil
* GVM
*/

#pragma once

#include <map>
#include <string>

using namespace std;

typedef int (*MyFuncPtrType)(int, int);

/*
* Natives class
* This class defines the concrete opcode functional implemetation
*/

class Natives
{
public:
	Natives(void);
	~Natives(void);
	// static function to intialize the map 
	static map<string, MyFuncPtrType> generateMap();

	// Add 2 arguments and returns the output
	static int addNative(int v1, int v2);

	/*	fields	*/
	// global map as a table for native ops function pointers
	static map<string, MyFuncPtrType> opcodes_pointers;
};

