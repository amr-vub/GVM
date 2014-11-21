/** 
* parser.cpp
* Amr Gamil
* GVM
*/

#include "stdafx.h"
#include "parser.h"
#include "Create_Structure_stmt.h"
#include "IMemory.h"
#include "Instruction.h"

extern short globalNum_ips;
short _CHUNK_GLOBAL = 0;

parser::parser(void)
{
}


parser::~parser(void)
{
}

// Function to tokinze the input string by a delimiter
// Check: http://www.oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
//
void parser::Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

/*
* parseInst Function
* This function recieves a strTokens, create the appropriate 
* instruction out of it based on the type argument.
* stmtToks : e.g. ["INST", "OPR", "idx", "OPCODE", "noOfips"]
*/
void parser::parseInst(vector<string> &stmtToks)
{
	// here also, as we only have small set of instructions types, and
	// to debug easily, we will use if statments instead of functions pointers
	if(stmtToks[1].compare("BGN") == 0)
	{
		// start inst		
		globalNum_ips = atoi(stmtToks[3].c_str());
		Create_Structure_stmt::createSink(stmtToks);
	}
	else if(stmtToks[1].compare("STP") == 0)
	{
		// stop inst
		Create_Structure_stmt::createStop(stmtToks);
	}
	else if(stmtToks[1].compare("OPR") == 0)
	{
		// operation inst
		Create_Structure_stmt::createOperation(stmtToks);
	}
	else if(stmtToks[1].compare("SWI") == 0)
	{
		// SWITCH inst
		Create_Structure_stmt::createSwitch(stmtToks);
	}
	else if(stmtToks[1].compare("SNK") == 0)
	{
		// sink inst
		Create_Structure_stmt::createSink(stmtToks);
	}
	else if(stmtToks[1].compare("SPL") == 0)
	{
		// split inst
		Create_Structure_stmt::createSplit(stmtToks);
	}
	else if(stmtToks[1].compare("CHN") == 0)
	{
		// contextChange inst
		Create_Structure_stmt::createContextChange(stmtToks);
	}
	else if(stmtToks[1].compare("RST") == 0)
	{
		// Context resotre inst
		Create_Structure_stmt::createContextRestore(stmtToks);
	}
	else if(stmtToks[1].compare("CNS") == 0)
	{
		// Constant inst
		Create_Structure_stmt::createConstant(stmtToks);
	}
}

/*
	Parse the link instruction
	e.g. : LINK chunk idx port -> chunk idx port
*/
void parser:: parseLink(vector<string> &stmtToks)
{
	// collecting info from the instruction
	short chunk1 = atoi(stmtToks[1].c_str());
	short chunk2 = atoi(stmtToks[5].c_str());

	int idx1 = atoi(stmtToks[2].c_str());
	int idx2 = atoi(stmtToks[6].c_str());

	short port1 = atoi(stmtToks[3].c_str());
	short port2 = atoi(stmtToks[7].c_str());

	int add1[2] = {chunk1, idx1};
	int *add2 = new int[2];
	add2[0] = chunk2;
	add2[1] = idx2;

	// query the memory for the source instruction to add destination address to it
	Instruction *inst= IMemory::get(add1);
	inst->distList[port1].push_back(make_tuple(add2,port2));
	IMemory::put(add1[0], add1[1], inst);
}

/*
	Parse CHUNK stmt

	CHUNK [0|1]
*/

void parser::parseChunk(vector<string> &stmtToks)
{
	// just fake for now a memory insertion
	_CHUNK_GLOBAL = atoi(stmtToks[1].c_str());
}

/*
	Parse LIT stmt

	LITR <inst> <port> <= <value>
*/
void parser::parseLit(vector<string> &stmtToks)
{
	int idx[2] = {_CHUNK_GLOBAL,  atoi(stmtToks[1].c_str())};
	short port = atoi(stmtToks[2].c_str());

	// TODO
	int value = atoi(stmtToks[4].c_str());

	// query the memory for the source instruction to add literals to it
	Instruction *inst= IMemory::get(idx);
	// TODO
	Datum d = Datum(value);
	d.token_Type = Datum::INT;
	inst->addLiterals(port, d);

	IMemory::put(idx[0], idx[1], inst);
}

// function to redirect the input stmt the correct parser
// \param: 
//		stmt: one line of the input file
void parser::parserMain(string &line)
{
	// Tokenize the input string by white spaces
	// e.g. : INST PB 0 2
	// gives vector of ["INST", "PB", "0", "2"]
	vector<string> strToks;
	Tokenize(line, strToks);
	if(strToks.empty())
		return;
	// no need here to implement a function pointer lookup table
	// if statment is used instead (easier to debug)
	string comp = strToks[0];
	if(comp.compare("INST") == 0)
	{
		// Instruction statment, delegate to parseInst
		parseInst(strToks);
	}
	else if(comp.compare("CHUNK") == 0)
	{
		// CHUNK statment
		// TODO
		parseChunk(strToks);		
	}
	else if(comp.compare("LINK") == 0)
	{
		// LINK statment
		// TODO
		parseLink(strToks);
	}
	else if(comp.compare("LITR") == 0)
	{
		// literal statment		
		parseLit(strToks);
	}
	else if(comp.compare("TRIV") == 0)
	{
		// trivial statment
	}
}


