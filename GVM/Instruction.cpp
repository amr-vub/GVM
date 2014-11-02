/** 
* Instruction.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "Instruction.h"
#include "Natives.h"

#include <iostream>

Instruction::Instruction(short &ch, int idx[2])
{
	this->idx[0] = idx[0];
	this->idx[1] = idx[1];
	this->chunk = ch;
	this->InstInx = 0;
	generateUniqueIdx();	
}

// deleting the pointers in the distList
Instruction::~Instruction(void)
{
	for (Tuple_vector::iterator it= this->distList.begin(); it != this->distList.end(); ++ it)
	{
		delete [] get<0>(*it);
	}
	this->distList.clear();
}

Instruction::Instruction(void)
{
}

/*

*/
void Instruction::addLiterals(short &port, Datum &value)
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
/*
void Instruction::execute2(Token_Type *tokens, Core *core)
{

}
*/
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
	this->tokenInputs = input;
}
/*
	preparing the args list
*/
vector<Datum> Operation::createArgsList(Token_Type* toks)
{
	vector<Datum> retArgs;
	if(this->literals.empty())
		// means that this op inst expect no literals
		for(int i=0; i<this->inputs;i++)
			retArgs.push_back(toks[i].data);
	else
	{		
		// this inst has literal inputs
		/*
		if(this->tokenInputs == 0)
			// no tokens are expeccted, just op between literals
			for(vector<tuple<short, int>>::iterator it = literals.begin() ; it!=literals.end(); ++it)
			{			
				retArgs.push_back(get<1>(*it));							
			}
		*/				
		// one literal and one token
		if(toks[0].tag->port < get<0>(literals.front()))
		{
			retArgs.push_back(toks[0].data);
			retArgs.push_back(get<1>(literals.front()));
		}
		else
		{
			retArgs.push_back(get<1>(literals.front()));
			retArgs.push_back(toks[0].data);
		}

		
	}
	return retArgs;

}

/*
	add literals to the op instruction and decrment the no of expected tokens
*/
void Operation::addLiterals(short &port, Datum &value)
{
	this->literals.push_back(make_tuple(port, value));
	this->tokenInputs--;
}

void Operation::execute(Token_Type *tokens, Core *core)
{
	// get the conceret function to be implemented based on the opcode of this objs
	MyFuncPtrType op_func_pointer = Natives::opcodes_pointers[this->opCode];

	Datum res;
	// prepare the args
	vector<Datum> args = createArgsList(tokens);
	// calling the function with the input arguments
	res = (*op_func_pointer)(args);


	// send the res to tokenizer
	core->tokenizer.wrapAndSend((this->distList), res, tokens[0].tag->conx);
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
void Sink::execute(Token_Type *tokens, Core *core)
{
	//std::cout<< core.dispatcher;
	// send the res to tokenizer
	core->tokenizer.wrapAndSend((this->distList), tokens[0].data, tokens[0].tag->conx);
	// freeing memory
	delete tokens;
}

/*
void Sink::execute2(Token_Type *tokens, Core *core)
{
	// send the res to tokenizer
	core->tokenizer->wrapAndSend((this->distList), tokens[0].data, tokens[0].tag->conx);
}
*/
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
void Switch::execute(Token_Type *tokens, Core *core)
{
	short port = tokens[0].tag->port;
	if(port == 0)
	{
		// the condition token has arrived
		// use it to index the destination list
		// and send all stored tokens recieved before
		// with the same context

		// first get all of the stored tokens
		long cx = tokens[0].tag->conx.conxId;
		vector<Token_Type*> toksV = core->tokenizer.swicther.getAllElement(cx);

		// then determine thier dest based on the recieved token's data
		int destIdx = tokens[0].data.iValue;
		Tuple_vector dest;

		// valide index
		if(this->distList.size() > destIdx)
		{
			dest.push_back(this->distList[destIdx]);
			if(!toksV.empty()) //at least one token exists
				core->tokenizer.swicther.sendToTokinzer(dest, toksV);
		}
		// freeing memory
		delete tokens;
	}
	else
	{
		// store this token till we recieve the condition token to specify it's dest
		core->tokenizer.swicther.addSwitchStorageElement(tokens);
	}	
}

/*********************** ContextChange Inst Part ******************************/

ContextChange::ContextChange(short chunk, int *indx, short &bds, short &rstors, int* to, int* ret)
	: Instruction(chunk, indx)
{
	binds = bds;
	restores = rstors;
	todest  = to;
	retDest = ret;
}

ContextChange::~ContextChange()
{
	// freeing memory
	delete [] this->todest;
	delete [] this->retDest;
	for(Tuple_vector::iterator it = this->distList.begin() ; it!=this->distList.end(); ++it)
	{
		// Tuple_vector = vector<tuple<int*,short>>
		tuple<int*,short> temp = (*it);
		distList.erase(it);
		delete [] get<0>(temp);
		if(distList.end() == it)
			break;
	}

}
/*
	ContextChange instruction execution
	- Change the context of the recieved tok
	- save the old cx to be restored when a context restore inst is executed
*/
void ContextChange::execute(Token_Type *tokens, Core *core)
{
	// delegate to the context manger obj to handel the context change execution details
	core->tokenizer.contextManager.bind_save(tokens[0], this->todest, this->retDest, this->binds, this->restores);
	// freeing memory
	delete tokens;
}

/*********************** ContextRestore Inst Part ******************************/

ContextRestore::ContextRestore(short &ch, int* ind) : Instruction(ch, ind)
{
}

ContextRestore::~ContextRestore()
{
}
/*
	ContextRestore instruction execution
	- Restore the context of the recieved tok
*/
void ContextRestore::execute(Token_Type *tokens, Core *core)
{
	// delegate to the context manger obj to handel the context restore execution details
	core->tokenizer.contextManager.restore(tokens[0]);
	//freeing memory
	delete tokens;
}

/*********************** Stop Inst Part ******************************/
Stop::Stop(short &ch, int idx[2]) : Instruction(ch, idx)
{
}

Stop::~Stop()
{
}

void Stop::execute(Token_Type *tokens, Core *core)
{
	core->tokenizer.sendStop(tokens);
}