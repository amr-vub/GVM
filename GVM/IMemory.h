/** 
* IMemory.h
* Amr Gamil
* GVM
*/

#pragma once
#include <map>


class Instruction;

using namespace std;

typedef map<int, Instruction*> ImemoryMapTyp;

/** 
* This class defines the Instruction memory, where
* all of the program instructions (dataflow graph) 
* are stored
*/

class IMemory
{
public:
	IMemory();
	~IMemory(void);
	/* given an instruction idx, return the respective instruction
	*	\param: 
			idx: instruction index
		\return
			the respective instruction
	*/
	static Instruction* get(int *idx);		

	static void put(short chunk, int idx, Instruction* inst);

	//fields
	//Core core;
	// a hash table that represents the memory data structure
	static map<int, Instruction*> Imemory[2];

};

