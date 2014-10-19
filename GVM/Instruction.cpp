/** 
* Instruction.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Instruction.h"
#include "Natives.h"
#include "Tokenizer.h"

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
	delete idx;
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

// Operation instruction part


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
