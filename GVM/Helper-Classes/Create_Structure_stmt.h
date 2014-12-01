/** 
* Create_Structure_stmt.h
* Amr Gamil
* GVM
*/

#pragma once
#include <vector>
#include <string>
#include "IMemory.h"

using namespace std;

/*
* This class provides the functionality for
	- DIS structural statments creation
	- Storage those stmt into the program memory
*/
class Create_Structure_stmt
{
public:
	Create_Structure_stmt(void);
	~Create_Structure_stmt(void);

	// Create an operation object and store it in the IMemory
	static void createOperation(vector<string> &strTokns, IMemory &_InstructionMemory);

	// Create a switch instruction and store it in the IMemory
	static void createSwitch(vector<string> &strTokns, IMemory &_InstructionMemory);

	// Create a sink instruction and store it in the IMemory
	static void createSink(vector<string> &strTokns, IMemory &_InstructionMemory);

	// Create a constant instruction and store it in the IMemory
	static void createConstant(vector<string> &strTokns, IMemory &_InstructionMemory);

	// Create a context change instruction and store it in the IMemory
	static void createContextChange(vector<string> &strTokns, IMemory &_InstructionMemory);
	
	// Create a context restore instruction and store it in the IMemory
	static void createContextRestore(vector<string> &strTokns, IMemory &_InstructionMemory);

	// Create a split instruction and store it in the IMemory
	static void createSplit(vector<string> &strTokns, IMemory &_InstructionMemory);

	// Create a stop instruction and store it in the IMemory
	static void createStop(vector<string> &strTokns, IMemory &_InstructionMemory);	
};

