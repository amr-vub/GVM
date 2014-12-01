/** 
* Create_Structure_stmt.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Create_Structure_stmt.h"
#include "Instruction.h"
#include "IMemory.h"

extern short _CHUNK_GLOBAL;

// save instruction in the IMemory
void putInMemory(short ch, int &idx,Instruction *instIp, IMemory &_InstructionMemory);

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
void Create_Structure_stmt::createOperation(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	string opCode = strTokns[3];
	short inputs = atoi(strTokns[4].c_str());	
	int indx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};	

	//create operation object
	Operation *op = new Operation(opCode, inputs, _CHUNK_GLOBAL, indx);	
	putInMemory(_CHUNK_GLOBAL, indx[1], op, _InstructionMemory);
	
}

/* Create a switch instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ["INST", "SWI", <idx>, <destination list>]
*/
void Create_Structure_stmt::createSwitch(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	int instIdx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};

	// create the SWI instruction	
	Switch *swi = new Switch(_CHUNK_GLOBAL, instIdx);

	short ch;
	int idx;
	// loop through the <destination list>
	for(vector<string>::iterator it = (strTokns.begin()+3); it!=strTokns.end(); ++it)
	{
		// get the chunk		
		ch = atoi((*it).c_str());
		// no need to check for out of index based on the assumption follow the standard pattern.
		it++;
		// get the inst indx
		idx = atoi((*it).c_str());

		// add to the switch dest list
		swi->destinationList.push_back(ch);
		swi->destinationList.push_back(idx);
	}
	putInMemory(_CHUNK_GLOBAL, instIdx[1], swi, _InstructionMemory);

}

/* Create a sink instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ["INST", "SNK", <idx>]
*/
void Create_Structure_stmt::createSink(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	// get the idx
	int indx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};

	// create the sink inst
	Sink *sink = new Sink(_CHUNK_GLOBAL, indx);

	// add to the memory
	putInMemory(_CHUNK_GLOBAL, indx[1], sink, _InstructionMemory);
}

/* Create a constant instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'CNS' '<idx>' '<=' '<value>']
*/
void Create_Structure_stmt::createConstant(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	// get the idx
	int indx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};
	// TODO 
	int value = atoi(strTokns[4].c_str());	

	// create the constant inst
	// TODO 
	Datum dat = Datum(value);
	dat.token_Type = Datum::INT;
	Constant<Datum> *constant = new Constant<Datum>(_CHUNK_GLOBAL, indx, dat);

	// add to the memory
	putInMemory(_CHUNK_GLOBAL, indx[1], constant, _InstructionMemory);
}

/* Create a ContextChange instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'CHN' '<idx>' '<binds>' '<restores>' '<to>' '<ret>']
*/
void Create_Structure_stmt::createContextChange(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	int indx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};
	short binds = atoi(strTokns[3].c_str());
	short restores = atoi(strTokns[4].c_str());
	int* toAdd =  new int[];
	toAdd[0] = atoi(strTokns[5].c_str());
	toAdd[1] = atoi(strTokns[6].c_str());

	int* retAdd =  new int[];
	retAdd[0] = atoi(strTokns[7].c_str());
	retAdd[1] = atoi(strTokns[8].c_str());

	// create the inst
	ContextChange* contextChange = new ContextChange(_CHUNK_GLOBAL, indx, binds, restores, toAdd, retAdd);

	// add to the memory
	putInMemory(_CHUNK_GLOBAL, indx[1], contextChange, _InstructionMemory);
}

/* Create a ContextRestore instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'RST' '<idx>']
*/
void Create_Structure_stmt::createContextRestore(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	int indx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};
	short ch = indx[0];

	// create the inst
	ContextRestore* contextRestore = new ContextRestore(ch, indx);

	// add to the memory
	putInMemory(_CHUNK_GLOBAL, indx[1], contextRestore, _InstructionMemory);
}

/*

	Create a split instruction and store it in the IMemory
	\param:
		strToknd: vector of strings. 
			e.g. [INST SPL <idx> <binds> <to> <merge>]

*/
void Create_Structure_stmt::createSplit(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	int indx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};
	short binds = atoi(strTokns[3].c_str());	
	int* toAdd =  new int[];
	toAdd[0] = atoi(strTokns[4].c_str());
	toAdd[1] = atoi(strTokns[5].c_str());

	int* mergeAdd =  new int[];
	mergeAdd[0] = atoi(strTokns[6].c_str());
	mergeAdd[1] = atoi(strTokns[7].c_str());

	// create the inst
	Split *split = new Split(_CHUNK_GLOBAL, indx, binds, toAdd, mergeAdd);
	// add to the memory
	putInMemory(_CHUNK_GLOBAL, indx[1], split, _InstructionMemory);
}

/* Create a Stop instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'STP' '<idx>']
*/
void Create_Structure_stmt::createStop(vector<string> &strTokns, IMemory &_InstructionMemory)
{
	int indx[2] = {_CHUNK_GLOBAL, atoi(strTokns[2].c_str())};	

	// create the inst
	Stop* stop = new Stop(_CHUNK_GLOBAL, indx);

	// add to the memory
	putInMemory(_CHUNK_GLOBAL, indx[1], stop, _InstructionMemory);

}

/* save instruction in the IMemory
	\param:
		inst: Abstract Instrucion pointer
*/
void putInMemory(short ch, int &idx,Instruction *instIp, IMemory &_InstructionMemory)
{
	Instruction* inst = instIp;
	_InstructionMemory.put(ch, idx, inst);
}