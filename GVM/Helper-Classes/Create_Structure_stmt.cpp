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
	short ch = 1;
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
	for(vector<string>::iterator it = (strTokns.begin()+3); it!=strTokns.end(); ++it)
	{
		// get the chunk		
		ch = atoi((*it).c_str());
		// no need to check for out of index based on the assumption follow the standard pattern.
		it++;
		// get the inst indx
		idx = atoi((*it).c_str());
		int *indx = new int[2];
		indx[0] = ch;
		indx[1] = idx;
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
	// TODO 
	int value = atoi(strTokns[4].c_str());

	short ch = 0;

	// create the constant inst
	// TODO 
	Datum dat = Datum(value);
	dat.token_Type = Datum::INT;
	Constant<Datum> *constant = new Constant<Datum>(ch, indx, dat);

	// add to the memory
	putInMemory(0, indx[1], constant);
}

/* Create a ContextChange instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'CHN' '<idx>' '<binds>' '<restores>' '<to>' '<ret>']
*/
void Create_Structure_stmt::createContextChange(vector<string> &strTokns)
{
	int indx[2] = {0, atoi(strTokns[2].c_str())};
	short binds = atoi(strTokns[3].c_str());
	short restores = atoi(strTokns[4].c_str());
	int* toAdd =  new int[];
	toAdd[0] = atoi(strTokns[5].c_str());
	toAdd[1] = atoi(strTokns[6].c_str());

	int* retAdd =  new int[];
	retAdd[0] = atoi(strTokns[7].c_str());
	retAdd[1] = atoi(strTokns[8].c_str());

	// create the inst
	ContextChange* contextChange = new ContextChange(0, indx, binds, restores, toAdd, retAdd);

	// add to the memory
	putInMemory(0, indx[1], contextChange);
}

/* Create a ContextRestore instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'RST' '<idx>']
*/
void Create_Structure_stmt::createContextRestore(vector<string> &strTokns)
{
	int indx[2] = {0, atoi(strTokns[2].c_str())};
	short ch = indx[0];

	// create the inst
	ContextRestore* contextRestore = new ContextRestore(ch, indx);

	// add to the memory
	putInMemory(0, indx[1], contextRestore);
}

/*

	Create a split instruction and store it in the IMemory
	\param:
		strToknd: vector of strings. 
			e.g. [INST SPL <idx> <binds> <to> <merge>]

*/
void Create_Structure_stmt::createSplit(vector<string> &strTokns)
{
	int indx[2] = {0, atoi(strTokns[2].c_str())};
	short binds = atoi(strTokns[3].c_str());	
	int* toAdd =  new int[];
	toAdd[0] = atoi(strTokns[4].c_str());
	toAdd[1] = atoi(strTokns[5].c_str());

	int* mergeAdd =  new int[];
	mergeAdd[0] = atoi(strTokns[6].c_str());
	mergeAdd[1] = atoi(strTokns[7].c_str());

	// create the inst
	Split *split = new Split(0, indx, binds, toAdd, mergeAdd);

	// add to the memory
	putInMemory(0, indx[1], split);
}

/* Create a Stop instruction and store it in the IMemory
	\param: 
		strToknd: vector of strings. 
			e.g. ['INST' 'STP' '<idx>']
*/
void Create_Structure_stmt::createStop(vector<string> &strTokns)
{
	int indx[2] = {0, atoi(strTokns[2].c_str())};
	short ch = indx[0];

	// create the inst
	Stop* stop = new Stop(ch, indx);

	// add to the memory
	putInMemory(0, indx[1], stop);

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