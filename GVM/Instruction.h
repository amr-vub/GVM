/** 
* Instruction.h
* Amr Gamil
* GVM
*/

#pragma once
#include <string>
#include <vector>
#include <tuple>
#include "PE.h"
#include "Core.h"

using namespace std;

//typedef Datum (*MyFuncPtrType)(vector<int>);

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
	virtual void execute(Token_Type **tokens, Core *core) = 0;

	//virtual void execute2(Token_Type *tokens, Core *core);

	// TODO
	virtual void addLiterals(short &port, Datum &value);

	//generate unique instIdx
	void generateUniqueIdx();

	/*	fields	*/

	// which chunk of memory this instruction is stored
	// TODO
	short chunk;

	// The index of the instruction
	int idx[2];

	// unique index for the inst based on the (chunk, instAdd)
	long InstInx;

	// List of destination nodes that the result 
	// of executing this instruction should be forwarded to
	MAP_Vector_Tuple distList;

	// vectors of literals
	// Only for Context change and Operation Instructions
	vector<tuple<short, Datum>> literals;
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
	void execute(Token_Type **tokens, Core *core);

	// preparing the args list
	vector<Datum> createArgsList(Token_Type** toks);

	// add literals to this inst
	void addLiterals(short &port, Datum &value);
	
	/*	fields	*/

	// the opCode that determins the operation to be executed
	string opCode;

	// total number of inputs
	short inputs;

	// total numbers of expected tokens
	short tokenInputs;
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
	
	void execute2(Token_Type *tokens, Core *core);
	// overriding the super method
	void execute(Token_Type **tokens, Core *core);

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
	Switch(short ch, int* idx);
	~Switch();

	// overriding the super method
	void execute(Token_Type **tokens, Core *core);

	/*	fields	*/	
	vector<int> destinationList;

};

/*
* ContextChange instruction
* Format :  INST CHN <idx> <binds> <restores> <to> <ret>
*	- <binds> how many argumet will be sent to the redirected address <to>
	- <restores> TODO
	- <to> Redirect the data to.
	- <ret> the return address after the cotext restore
*/
class ContextChange : public Instruction
{
public:
	ContextChange(short chunk, int* indx, short &bds, short &rstors, int* to, int* ret);
	~ContextChange();

	// overriding the super method
	void execute(Token_Type **tokens, Core *core);

	// add literals to this inst
	void addLiterals(short &port, Datum &value);

	/*	fields	*/	
	short binds;
	short restores;
	int* todest;
	int* retDest;


};

/*
* ContextRestore instruction
* Restore a prevoiusly changed context. e.g. a function return
* Format : INST RST <idx>
*/
class ContextRestore : public Instruction
{
public:
	ContextRestore(short &ch, int* ind);
	~ContextRestore();
	// overriding the super method
	void execute(Token_Type **tokens, Core *core);

};

/*
	Split Instruction
	This instruction is mainly for parallizing for loop
	Given a compound data, it splits them into different 
	tokens and send each to the tokenizer with new cx.
	Format: INST SPL <idx> <binds> <to> <merge>
*/
class Split : public Instruction
{
public:
	Split(short chunk, int* idx, short binds, int *todest, int *mergeDest);
	~Split();

	// overriding the super method
	void execute(Token_Type **tokens, Core *core);

	/*	fields	*/
	short binds;
	int *todest;
	int *mergeDest;
private:
	void doSplitWork(Token_Type* tok, Token_Type* tokens, short portIdx, Core *core);
};

/*
	Stop Instruction
*/
class Stop : public Instruction
{
public:
	Stop(short &ch, int idx[2]);
	~Stop();
	// overriding the super method
	void execute(Token_Type **tokens, Core *core);
};


/*
* Constant Instruction
* Forwards the recieved value to all of it's destination when triggered
* by a input reception in port 0
* Format : INST CNS <idx> <= <value>
*/
template<class T>
class Constant : public Instruction
{
public:
	Constant();
	Constant(short &ch, int* indx, T &value);
	~Constant();
	// overriding the super method
	void execute(Token_Type **tokens, Core *core);

	/*	fields	*/

	// The recieved value
	T value;

};

template<class T>
Constant<T>::Constant()
{
}

template<class T>
Constant<T>::Constant(short &ch, int* indx, T &value) : Instruction(ch, indx)
{
	this->value = value;
}

template<class T>
Constant<T>::~Constant()
{
}

/*
	Constant istruction has the following formate:
		INST CNS <idx> <= <value>
	It redirects the value to all of it's destination once
	it recieves an input at port 0
*/
template<class T>
void Constant<T>::execute(Token_Type **tokens, Core *core)
{
	short port = tokens[0]->tag->port;
	Context cx = tokens[0]->tag->conx;

	if(0 == port)
		core->tokenizer.wrapAndSend(this->distList[port], this->value, cx);
	// freeing memory
	delete tokens[0];
}