/** 
* Instruction.h
* Amr Gamil
* GVM
*/

#pragma once
#include <string>
#include <vector>
#include <tuple>
#include "Core.h"
#include "Token.h"
#include "PE.h"

using namespace std;

typedef int (*MyFuncPtrType)(int, int);

/*
* This class defines the base structure of instructions that Imemory
* can store.
* According to the DIS design, we have basically 7 instructions
	- Sink
	- Constant
	- Operation
	- Switch
	- ContextChange
	- ContextRestore
	- Split
	All of those onstruction will be have thier own class, and 
	will inherit this base class
*/
class Instruction
{
public:
	Instruction();
	Instruction(short &ch, int idx[2]);
	~Instruction();
	// Abstract function that has to be implemeted by all subtypes
	// TODO : add virtual
	virtual void execute(Token<int> *tokens, Core &core) = 0;

	//generate unique instIdx
	void generateUniqueIdx();

	/*	fields	*/

	// which chunk of memory this instruction is stored
	short chunk;

	// The index of the instruction
	int idx[2];

	// unique index for the inst based on the (chunk, instAdd)
	long InstInx;

	// List of destination nodes that the result 
	// of executing this instruction should be forwarded to
	Tuple_vector distList;
};

/*
* Operation instruction
* The  core of the Instruction set, uses to represent and execute
* all native suported operation
* Format :  INST OPR <idx> <opCode> <inputs>
*/
class Operation : public Instruction
{
public:
	Operation();
	Operation(string &opCode, short &input, short &chunk, int idx[2]);
	~Operation();

	// overriding the super method
	void execute(Token<int> *tokens, Core &core);
	
	/*	fields	*/

	// the opCode that determins the operation to be executed
	string opCode;

	// total number of inputs
	short inputs;
};

/*
* Sink instruction.
* Forwards the input to thier destination.
* Format : INST SNK <idx>
*/
class Sink : public Instruction
{
public:
	Sink(short ch, int *idx);
	~Sink();
	
	// overriding the super method
	void execute(Token<int> *tokens, Core &core);

	/*	fields	*/


	//
};


/*
* Switch instruction
* Switche instruction redirects of data based on a condition.
* Format : INST SWI <idx> <destination list>
*/
class Switch : public Instruction
{
public:
	Switch();
	~Switch();

	// overriding the super method
	void execute(Token<int> *tokens, Core &core);

	/*	fields	*/	
	//int *destinationList;

};