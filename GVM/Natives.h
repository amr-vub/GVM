/** 
* Natives.h
* Amr Gamil
* GVM
*/

#pragma once

#include <map>
#include <string>
#include <vector>
#include "Datum.h"

using namespace std;

typedef Datum (*MyFuncPtrType)(vector<Datum>);

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
	static Datum addNative(vector<Datum> args);

	// Sub 2 arguments and returns the output
	static Datum subNative(vector<Datum> args);

	// Mul 2 arguments and returns the output
	static Datum mulNative(vector<Datum> args);

	// Return true if thefirst input is greater than right input.
	static Datum moreNative(vector<Datum> args);

	// Return true if thefirst input is smaller than right input.
	static Datum lessNative(vector<Datum> args);

	// Return the input as an int
	static Datum intNative(vector<Datum> args);

	/*	fields	*/
	// global map as a table for native ops function pointers
	static map<string, MyFuncPtrType> opcodes_pointers;
};

