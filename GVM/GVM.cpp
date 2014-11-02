/** 
* GVM.cpp
* Amr Gamil
* GVM
*/
// GVM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Token_Type.h"
#include "Core.h"
#include "parser.h"
#include "IMemory.h"
#include "MatchingUnit.h"
#include "TokenDispatcher.h"
#include "Scheduler.h"
#include "Datum.h"

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
	// create the only seq core		
	Core core = Core(0);
	// start instruction address
	int indxStrAdd[2] = {0,0};
	// get a context
	Context *firstCx = core.conxObj.getUniqueCx(core.coreID);
	// get the user input, and for each, create a token
	for(int i=0;i<globalNum_ips;i++)
	{
		cin >> data;
		Tag * tag = new Tag(*firstCx, 0, indxStrAdd);
		Datum datum = Datum(data);
		datum.token_Type = Datum::INT;
		Token_Type *tok = new Token_Type(datum, tag);
		core.inbox.push_back(tok);
	}

	start = std::clock();
	// start the actual work
	core.start();
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Total time: "<< duration <<'\n';

	// when it returns from start, the token that resides in the inbox
	// will be the program result
	Token_Type *tok = core.inbox.front();
	cout<< "the result is : \n";
	cout<< tok->data.iValue;
	core.inbox.clear();

	// freeing memory
	delete tok;

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
