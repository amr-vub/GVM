/** 
* parser.h
* Amr Gamil
* GVM
*/

#pragma once
#include <vector>
#include <string>
#include "Instruction.h"

typedef void (*FunctionTypeVoid)(char*);
typedef vector<string, FunctionTypeVoid> Vector_type;

using namespace std;

/*
* Parser class.
* This class defines the parser class which is responsible to parse
* the input file and populate the instruction memeroy with the 
* corresponding 'machine code'.
*/
class parser
{
public:
	parser(void);

	~parser(void);

	// fcuntion to redirect the input stmt the correct parser
	static void parserMain(string &stmt);

	//function to parse the input in case of INST
	static void parseInst(vector<string> &stmtToks);

	//function to parse the input in case of CHUNK
	// TODO
	 //static void parseChunk(vector<string> &stmtToks);

	 //function to parse the input in case of LIT
	 // TODO
	 //static void parseLit(vector<string> &stmtToks);

	 //function to parse the input in case of LINK
	 static void parseLink(vector<string> &stmtToks);

	 //function to parse the input in case of TRIV
	 // TODO
	 //static void parseTriv(vector<string> &stmtToks);

	// function to intialiaze the vector_func_pointer
	static Vector_type generate_vector_parser_pointer();

	// Abstract fucntion to be implemented by derived class 
	// parse the input Instruction
	//virtual void parse(Instruction *ins) = 0;

	//
	static void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters);

	//array of function pointers
	//static Vector_type vector_parser_pointer;
};

