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
	~IMemory();

	// adding a copy constructor for managing the map of pointers copy
	IMemory(const IMemory &obj);
	/* given an instruction idx, return the respective instruction
	*	\param: 
			idx: instruction index
		\return
			the respective instruction
	*/
	Instruction* get(int *idx);		

	void put(short chunk, int idx, Instruction* inst);

	//fields
	//Core core;
	// a hash table that represents the memory data structure
	map<int, Instruction*> Imemory[2];

};

