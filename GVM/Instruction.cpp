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
	for(auto mp: this->distList)
	{
		for (auto vect : mp.second)
		{
			delete [] get<0>(vect);
		}
		mp.second.clear();
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
	this->InstInx <<= 31;

	//put the InstAdd in the LSB
	long temp = 0;
	temp = this->idx[1];
	this->InstInx |= temp;
}
bool Instruction::isSwitch()
{
	return false;
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
vector<Datum> Operation::createArgsList(Token_Type** toks, short ips)
{
	vector<Datum> retArgs;	
	if(this->literals.empty())
		// means that this op inst expect no literals
			if(this->inputs == 0)
				// means that it was an array op
				for(int i=0; i<ips;i++)
					retArgs.push_back(toks[i]->data);
			else
				for(int i=0; i<this->inputs;i++)
					retArgs.push_back(toks[i]->data);
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
		if(toks[0]->tag->port < get<0>(literals.front()))
		{
			retArgs.push_back(toks[0]->data);
			retArgs.push_back(get<1>(literals.front()));
		}
		else
		{
			retArgs.push_back(get<1>(literals.front()));
			retArgs.push_back(toks[0]->data);
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

void Operation::execute(Token_Type **tokens, Core *core)
{
	// get the conceret function to be implemented based on the opcode of this objs
	MyFuncPtrType op_func_pointer = Natives::opcodes_pointers[this->opCode];
	Datum res;
	short ips = 0;
	if(this->opCode == "array")
	{
		unordered_map<unsigned long long, short>::iterator tokenIt = 
			core->matchUnit.arrayInputsStore.find(tokens[0][0].tag->tokenID);
		ips = tokenIt->second;
		core->matchUnit.arrayInputsStore.erase(tokenIt);
	}
	// prepare the args
	vector<Datum> args = createArgsList(tokens, ips);
	// calling the function with the input arguments
	res = (*op_func_pointer)(args);


	// send the res to tokenizer
	core->tokenizer.wrapAndSend((this->distList[0]), res, tokens[0][0].tag->conx, core->coreID, 
		tokens[0][0].tag->token_executor_coreID);
}
// OPR is very tedious to be stealed, as it requires
// at least the sharing of the token tables, but in case of one token input,
// it is an independant instruction
bool Operation::isINDependant()
{
	if(this->tokenInputs == 1)
		return true;
	return false;
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
void Sink::execute(Token_Type **tokens, Core *core)
{
	//std::cout<< core.dispatcher;
	short ex_CoreId = tokens[0][0].tag->token_executor_coreID;
	short crID = core->coreID;	
	// send the res to this core tokenizer
	core->tokenizer.wrapAndSend((this->distList[tokens[0][0].tag->port]), tokens[0][0].data, 
		tokens[0][0].tag->conx, crID , ex_CoreId);							
	// freeing memory
	delete tokens[0];
}

// no local data dependancy
bool Sink::isINDependant()
{
	return true;
}

/*
void Sink::execute2(Token_Type *tokens, Core *core)
{
// send the res to tokenizer
core->tokenizer->wrapAndSend((this->distList), tokens[0][0].data, tokens[0][0].tag->conx);
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
// returns true if this is a switch instruction
// just for identifying the dynamic number of expected inputs
// for SWI to be used in garbage collection purposes
bool Switch::isSwitch()
{
	return true;
}
/*
Switch instruction execution
- The switch instruction has list of destination, and a condition input.
- It recieves the condition input on it's port 0.
- This condition token's data value determins the index of the chosen dest
- All tokens recieved before the condition var are stored
*/
void Switch::execute(Token_Type **tokens, Core *core)
{
	short port = tokens[0][0].tag->port;
	if(port == 0)
	{
		// the condition token has arrived
		// use it to index the destination list
		// and send all stored tokens recieved before
		// with the same context

		// first get all of the stored tokens

		vector<Token_Type*> toksV = core->tokenizer.swicther.
			getAllElement(tokens[0][0].tag->tokenID );
		// NEW: Switch has to forward all of its recieved tokens
		toksV.push_back(tokens[0]);
		// then determine thier dest based on the recieved token's data
		int destIdx = tokens[0][0].data.iValue * 2;
		int *indx = new int[2];
		indx[0] = this->destinationList[destIdx];
		indx[1] = this->destinationList[destIdx+1];		

		// valide index
		if(this->destinationList.size() > destIdx)
		{			
			// check if all of the expected inputs have been rec
			if(toksV.size() < this->inputs)
			{
				short remainingInputs = this->inputs - toksV.size();
				core->tokenizer.swicther.storeDest(tokens[0], remainingInputs, indx);
			}
			// loop through the saved existing tokens, for each 
			// determine the destination address and the port num
			// based on the recieved index and SWITCH <destination LIST>
			for(auto tok: toksV){
				Vector_Tuple dest;
				short port = tok->tag->port;
				dest.push_back(make_tuple(indx, port));
				core->tokenizer.wrapAndSend(dest, tok->data, tok->tag->conx, core->coreID
					,core->coreID);
			}
		}
		// freeing memory
		delete tokens[0];
	}
	else if(core->tokenizer.swicther.alreadyExists(tokens[0]))
	{
		// this token is mapped to an already selected dest, as it arrives after the port 0 token!
		int* indx = get<1>(core->tokenizer.swicther.updateStoredDest(tokens[0]));
		Vector_Tuple dest;
		short port = tokens[0]->tag->port;
		dest.push_back(make_tuple(indx, port));
		core->tokenizer.wrapAndSend(dest, tokens[0]->data, tokens[0]->tag->conx, core->coreID, core->coreID);
	}
	else
	{
		// store this token till we recieve the condition token to specify it's dest
		core->tokenizer.swicther.addSwitchStorageElement(*tokens);
	}	
}

// SWI instruction has local data storage which is also tediuos to share
bool Switch::isINDependant()
{
	return false;
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

}

// add literals to this inst
void ContextChange::addLiterals(short &port, Datum &value)
{
	this->literals.push_back(make_tuple(port, value));
}

/*
ContextChange instruction execution
- Change the context of the recieved tok
- save the old cx to be restored when a context restore inst is executed
*/
void ContextChange::execute(Token_Type **tokens, Core *core)
{
	// Checks is the instruction already has literals or not
	if(this->literals.empty())
		// delegate to the context manger obj to handel the context change execution details
			core->tokenizer.contextManager.bind_save(tokens[0][0], this->todest, this->retDest, 
			this->binds, this->restores, this->InstInx);
	else
	{
		// means that there exist at least one literal, then compare the binds to the literals size
		for(auto lit : this->literals)
		{
			// prepare the literal as a token			
			short port = get<0>(lit);
			Datum value = get<1>(lit);
			Tag *tag = new Tag(tokens[0]->tag->conx, port, tokens[0]->tag->instAdd, core->coreID);
			Token_Type tok2 =  Token_Type(value, tag);
			// then send delegate
			core->tokenizer.contextManager.bind_save(tok2, this->todest, this->retDest, 
				this->binds, this->restores, this->InstInx);			
		}
		this->literals.clear();
		core->tokenizer.contextManager.bind_save(tokens[0][0], this->todest, this->retDest, 
			this->binds, this->restores, this->InstInx);
	}
	// freeing memory
	delete tokens[0];
}

// CHG can be stolen iff the number of bind is == 1
// else, contextMap has to be checked for previous entries
// for the same tokenID. This is done in the tokenizer level
bool ContextChange::isINDependant()
{
	if(this->binds == 1)
		return true;
	return false;
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
void ContextRestore::execute(Token_Type **tokens, Core *core)
{
	// delegate to the context manger obj to handel the context restore execution details
	core->tokenizer.contextManager.restore(tokens[0][0]);
	//freeing memory
	delete tokens[0];
}

// All of the restore information is saved locally in the restoreMap
bool ContextRestore::isINDependant()
{
	return false;
}

/*********************** Split Inst Part ******************************/

Split::Split(short chunk, int* idx, short binds, int *todest, int *mergeDest) : Instruction(chunk, idx)
{
	this->inputs = binds;
	this->tokenInputs = binds;
	this->todest = todest;
	this->mergeDest = mergeDest;
}

Split::~Split()
{
	// freeing memory
	delete [] this->todest;
	delete [] this->mergeDest;
}
/*

*/
void Split::execute(Token_Type **tokens, Core *core)
{
	// TODO : think about abstraction
	//returnVectorOfDatums(tokens[0][0].data);

	Token_Type *tok = new Token_Type();
	tok->tag = new Tag();
	int portIdx= 0;

	switch(tokens[0][0].data.token_Type)
	{
	case Datum::I_VECTOR:
		{
			//loop through array elements and for each, create a new token with new context
			for(vector<int>::iterator it = tokens[0][0].data.iValue_v.begin(); it!=tokens[0][0].data.iValue_v.end(); ++it)
			{
				tok->data = Datum(*it);
				tok->data.token_Type = Datum::INT;
				doSplitWork(tok,  tokens, portIdx++, core);				
			}
		}
		break;
	case Datum::F_VECTOR:
		{
			//loop through array elements and for each, create a new token with new context
			for(vector<float>::iterator it = tokens[0][0].data.fValue_v.begin(); it!=tokens[0][0].data.fValue_v.end(); ++it)
			{
				tok->data = Datum(*it);
				tok->data.token_Type = Datum::FLOAT;
				doSplitWork(tok,  tokens, portIdx++, core);
			}
		}
		break;
	case Datum::B_VECTOR:
		{
			//loop through array elements and for each, create a new token with new context
			for(vector<bool>::iterator it = tokens[0][0].data.bValue_v.begin(); it!=tokens[0][0].data.bValue_v.end(); ++it)
			{
				tok->data = Datum(*it);
				tok->data.token_Type = Datum::BOOLEAN;
				doSplitWork(tok,  tokens, portIdx++, core);
			}
		}
		break;
	default:
		printf("_____ERORR_____ Split\n");
	}
	// update the array operation, i.e. the merge instruction, with the size
	// of the splited array as it's new input
	Token_Type** tokensArr;
	tokensArr = new Token_Type*[portIdx];	
	TokenTableValue savedTKstruct = {
		savedTKstruct.inputs = portIdx,
		savedTKstruct.Indx = 0,
		savedTKstruct.tokenArr = tokensArr
	};
	//workaround to genrate the right token Id mix
	tok->tag->instAdd[0] = mergeDest[0];
	tok->tag->instAdd[1] = mergeDest[1];
	tok->tag->generateUniqeInstIdx();
	tok->tag->generateUniqeMix();
	// then store it in the token table
	core->matchUnit.tokenTable[tok->tag->tokenID] =	savedTKstruct;
	// save also the number of expected inputs for thi array op
	core->matchUnit.arrayInputsStore[tok->tag->tokenID] = portIdx;
	// free memory
	//delete [] tokens;
}

/*
A function just to encapsulte the dulicated code
Prepare the token, create new context and call the bind_send method
*/
void Split::doSplitWork(Token_Type* tok, Token_Type** tokens, short portIdx, Core *core)
{
	*tok->tag = *tokens[0]->tag;
	// change the port to match the element index
	//tok->tag->port = portIdx;
	// generate new context
	Context *new_cx = core->conxObj.getUniqueCx(core->coreID);
	// send each element in the array to the same instruction instance
	short crID = core->tokenizer.contextManager.bind_send(*tok, this->todest, portIdx, 0, this->mergeDest, 1, new_cx, -1);
	// then send the args

	for(int i = 1; i<this->inputs; i++)
	{
		Vector_Tuple dest;
		dest.push_back(make_tuple(this->todest,tokens[i]->tag->port));
		// TODO, change the core id				
		core->tokenizer.wrapAndSend(dest, tokens[i]->data, *new_cx, crID, crID);
	}
}

// split is treated the same as CHG, but here binds are always
// bigger than one, so, delegate this to the tokinzer
bool Split::isINDependant()
{
	if(this->tokenInputs == 1) // just in rare cases of the binds == 1
		return true;
	return false;
}
/*********************** Stop Inst Part ******************************/
Stop::Stop(short &ch, int idx[2]) : Instruction(ch, idx)
{
}

Stop::~Stop()
{
}

void Stop::execute(Token_Type **tokens, Core *core)
{
	core->tokenizer.sendStop(*tokens);
}

// no need to steal this
bool Stop::isINDependant()
{
	return false;
}
