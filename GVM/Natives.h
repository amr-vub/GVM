/** 
* Natives.h
* Amr Gamil
* GVM
*/

#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

typedef int (*MyFuncPtrType)(vector<int>);

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
	static int addNative(vector<int> args);

	// Sub 2 arguments and returns the output
	static int subNative(vector<int> args);

	// Mul 2 arguments and returns the output
	static int mulNative(vector<int> args);

	// Return true if thefirst input is greater than right input.
	static int moreNative(vector<int> args);

	// Return the input as an int
	static int intNative(vector<int> args);

	/*	fields	*/
	// global map as a table for native ops function pointers
	static map<string, MyFuncPtrType> opcodes_pointers;
};

