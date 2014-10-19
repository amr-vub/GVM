/** 
* GVM.cpp
* Amr Gamil
* GVM
*/
// GVM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Core.h"
#include "parser.h"
#include "IMemory.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isLegalLine(string &line);

int _tmain(int argc, _TCHAR* argv[])
{	

	// open the instrucion file to parse
	fstream instFile;
	instFile.open(argv[1]);

	string line;

	if(instFile.is_open())
	{
		// read the file line by line
		while(getline(instFile, line))
		{
			if(!isLegalLine(line))
				continue;
			parser::parserMain(line);
		}
	}
	/*
	// just for trials
	int idx[2] = {0,2};

	Instruction *inst = IMemory::get(idx);

	Context cx = Context(0);
	Token<int> tok1 = Token<int>(2,Tag(cx,0, idx));
	Token<int> tok2 = Token<int>(3,Tag(cx,1, idx));
	Token<int> toks[] = {tok1, tok2};

	inst->execute(toks, Core());
	// end of trial part
	*/

	return 0;
}

/* Function to parse the line and skip if it starts with '$'
   ,which is the comment char in DIS, and to delete special chars e.g. \t (tabs)
*/
bool isLegalLine(string &line)
{
	for(string::iterator it = line.begin(); it<line.end();++it)
	{
		// this fixes the problem of "\t$ ........"
		if((*it) == '$')
			return false;
		// this fixed the problem of e.g. "\tINST...."
		if((*it) == '\t')
		{
			line.erase(it);
			if(it == line.end())
				break;
		}
	}
	return true;
}
