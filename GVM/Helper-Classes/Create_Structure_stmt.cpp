/** 
* Create_Structure_stmt.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Create_Structure_stmt.h"
#include "Instruction.h"
#include "IMemory.h"

// save instruction in the IMemory
void putInMemory(short ch, int &idx,Instruction *instIp);

Create_Structure_stmt::Create_Structure_stmt(void)
{
}


Create_Structure_stmt::~Create_Structure_stmt(void)
{
}

/* Create an operation object and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ["INST", "OPR", "idx", "OPCODE", "noOfips"]
*/
void Create_Structure_stmt::createOperation(vector<string> &strTokns)
{
	string opCode = strTokns[3];
	short inputs = atoi(strTokns[4].c_str());
	short ch = 0;
	int indx[2] = {ch, atoi(strTokns[2].c_str())};	

	//create operation object
	Operation *op = new Operation(opCode, inputs, ch, indx);	
	putInMemory(ch, indx[1], op);
	
}

/* Create a switch instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ["INST", "SWI", <idx>, <destination list>]
*/
void Create_Structure_stmt::createSwitch(vector<string> &strTokns)
{
	int instIdx[2] = {0, atoi(strTokns[2].c_str())};

	// create the SWI instruction	
	Switch *swi = new Switch(0, instIdx);

	short ch;
	int idx;
	// loop through the <destination list>
	for(vector<string>::iterator it = (strTokns.begin()+2); it!=strTokns.end(); ++it)
	{
		// get the chunk		
		ch = atoi((*it).c_str());
		// no need to check for out of index based on the assumption follow the standard pattern.
		it++;
		// get the inst indx
		idx = atoi((*it).c_str());
		int indx[2] = {ch, idx};
		// add to the inst dest list
		swi->distList.push_back(make_tuple(indx, 0));
	}
	putInMemory(0, instIdx[1], swi);

}

/* Create a sink instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ["INST", "SNK", <idx>]
*/
void Create_Structure_stmt::createSink(vector<string> &strTokns)
{
	// get the idx
	int indx[2] = {0, atoi(strTokns[2].c_str())};

	// create the sink inst
	Sink *sink = new Sink(0, indx);

	// add to the memory
	putInMemory(0, indx[1], sink);
}

/* Create a constant instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'CNS' '<idx>' '<=' '<value>']
*/
void Create_Structure_stmt::createConstant(vector<string> &strTokns)
{
	// get the idx
	int indx[2] = {0, atoi(strTokns[2].c_str())};
	int value = atoi(strTokns[4].c_str());
	short ch = 0;

	// create the sink inst
	Constant<int> *constant = new Constant<int>(ch, indx, value);

	// add to the memory
	putInMemory(0, indx[1], constant);
}

/* save instruction in the IMemory
	\param:
		inst: Abstract Instrucion pointer
*/
void putInMemory(short ch, int &idx,Instruction *instIp)
{
	Instruction* inst = instIp;
	IMemory::put(ch, idx, inst);
}