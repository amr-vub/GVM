/** 
* Create_Structure_stmt.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Create_Structure_stmt.h"
#include "Instruction.h"
#include "IMemory.h"


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
	int indx[2] = {0, atoi(strTokns[2].c_str())};
	short ch = 0;

	//create operation object
	Operation *op = new Operation(opCode, inputs, ch, indx);	
	Instruction *ints = op;
	//update the program memory
	IMemory::put(ch, indx[1], ints);
	
}

