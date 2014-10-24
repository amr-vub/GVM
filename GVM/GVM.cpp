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
#include "MatchingUnit.h"
#include "TokenDispatcher.h"
#include "Scheduler.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <ctime>

short globalNum_ips;

using namespace std;

bool isLegalLine(string &line);

int _tmain(int argc, _TCHAR* argv[])
{	
	clock_t start;
    double duration;

	int data;	

	//Core core = Core();

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
			
	Core core = Core(0);
	/*
	TokenDispatcher* dispatcher = new TokenDispatcher(core);
	MatchingUnit *matchUnit = new MatchingUnit(core);
	Scheduler *sch = new Scheduler(core);	
	Tokenizer* tokenizer = new Tokenizer(core);	

	core.dispatcher = dispatcher;
	core.matchUnit = matchUnit;
	core.sch = sch;
	core.tokenizer = tokenizer;
	*/
	int indxStrAdd[2] = {0,0};

	Context *firstCx = core.conxObj.getUniqueCx(core.coreID);

	for(int i=0;i<globalNum_ips;i++)
	{
		cin >> data;
		Tag * tag = new Tag(*firstCx, 0, indxStrAdd);
		Token<int> *tok = new Token<int>(data, tag);
		core.inbox.push_back(tok);
	}

	start = std::clock();
	core.start();
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Total time: "<< duration <<'\n';

	// when it returns from start, the token that resides in the inbox
	// will be the program result
	cout<< "the result is : \n";
	cout<< core.inbox.front()->data;

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
