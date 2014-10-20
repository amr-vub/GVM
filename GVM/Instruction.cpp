/** 
* Instruction.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Instruction.h"
#include "Natives.h"

Instruction::Instruction(short &ch, int idx[2])
{
	this->idx[0] = idx[0];
	this->idx[1] = idx[1];
	this->chunk = ch;
	this->InstInx = 0;
	generateUniqueIdx();	
}


Instruction::~Instruction(void)
{
}

Instruction::Instruction(void)
{
}

void Instruction::generateUniqueIdx()
{
	// put the chunck value in the MSB
	this->InstInx = this->idx[0];
	this->InstInx <<= 32;

	//put the InstAdd in the LSB
	long temp = 0;
	temp = this->idx[1];
	this->InstInx |= temp;
}

/*********************** Operation Part ******************************/

Operation::Operation()
{
}

Operation::~Operation()
{
}

Operation::Operation(string &opCode, short &input,
					 short &chunk, int idx[2]) : Instruction(chunk, idx)
{
	this->inputs = input;
	this->opCode = opCode;
}

void Operation::execute(Token<int> *tokens, Core &core)
{
	// get the conceret function to be implemented based on the opcode of this objs
	MyFuncPtrType op_func_pointer = Natives::opcodes_pointers[this->opCode];

	// calling the function with the input arguments
	int res = (*op_func_pointer)(tokens[0].data, tokens[1].data);

	// send the res to tokenizer
	core.tokenizer->wrapAndSend((this->distList), res, tokens[0].tag.conx);
}


/*********************** SINK Inst Part ******************************/


Sink::Sink(short ch, int *idx) : Instruction(ch, idx)
{
	
}

Sink::~Sink()
{
}

/*
	Sink instruction simply forwad it's inputs to thier dest
*/
void Sink::execute(Token<int> *tokens, Core &core)
{
	// send the res to tokenizer
	core.tokenizer->wrapAndSend((this->distList), tokens[0].data, tokens[0].tag.conx);
}

/*********************** SWITCH Inst Part ******************************/

Switch::Switch(short ch, int* idx) : Instruction(ch, idx)
{	
}

Switch::Switch()
{	
}

Switch::~Switch()
{	
}
/*
	Switch instruction execution
	- The switch instruction has list of destination, and a condition input.
	- It recieves the condition input on it's port 0.
	- This condition token's data value determins the index of the chosen dest
	- All tokens recieved before the condition var are stored
*/
void Switch::execute(Token<int> *tokens, Core &core)
{
	short port = tokens[0].tag.port;
	if(port == 0)
	{
		// the condition token has arrived
		// use it to index the destination list
		// and send all stored tokens recieved before
		// with the same context

		// first get all of the stored tokens
		long cx = tokens[0].tag.conx.conxId;
		vector<Token<int>> toksV = core.tokenizer->swicther->getAllElement(cx);

		// then determine thier dest based on the recieved token's data
		int destIdx = tokens[0].data;
		Tuple_vector dest;

		// valide index
		if(this->distList.size() > destIdx)
		{
			dest.push_back(this->distList[destIdx]);
			if(!toksV.empty()) //at least one token exists
				core.tokenizer->swicther->sendToTokinzer(dest, toksV);
		}
	}
	else
	{
		// store this token till we recieve the condition token to specify it's dest
		core.tokenizer->swicther->addSwitchStorageElement(tokens[0]);
	}	
}

