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
#include <boost\thread\thread.hpp>

short globalNum_ips;

using namespace std;

bool isLegalLine(string &line);

int _tmain(int argc, _TCHAR* argv[])
{	
	clock_t start;
    double duration;		

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
	// create a core per processor
	short coreNumbers = 4;
	vector<Core*> coreList;
	for (int i = 0; i < coreNumbers; i++)
	{
		Core *core = new Core(i);
		core->tokenizer.coreList.push_back(core);
		coreList.push_back(core);
	}
	//printf("size : %d \n", coreList.front()->tokenizer.coreList.front()->coreID);
	Core *core = coreList.front()->tokenizer.coreList.front();
	// start instruction address
	int indxStrAdd[2] = {0,0};
	string input;
	
	// get a context
	Context *firstCx = core->conxObj.getUniqueCx(core->coreID);
	// get the user input, and for each, create a token
	for(int i=0;i<globalNum_ips;i++)
	{
		vector<string> strToks;
		getline(cin,input);
		parser::Tokenize(input,strToks);
		Tag * tag = new Tag(*firstCx, i, indxStrAdd);
		if(strToks.size() == 1)
		{			
			Datum datum = Datum(atoi(strToks[0].c_str()));
			datum.token_Type = Datum::INT;
			Token_Type *tok = new Token_Type(datum, tag);
			core->inbox.push_back(tok);
		}
		else if(strToks.size() > 1)
		{
			Datum datum = Datum();
			datum.token_Type = Datum::I_VECTOR;
			for (int i = 0; i < strToks.size(); i++)
			{
				datum.iValue_v.push_back(atoi(strToks[i].c_str()));
			}						
			Token_Type *tok = new Token_Type(datum, tag);
			core->inbox.push_back(tok);
		}

	}

	start = std::clock();
	// start the actual work
	for (int i = 0; i < coreList.size(); i++)
	{
		coreList[i]->run();
	}
	// wait for all cores to finish
	for (int i = 0; i < coreList.size(); i++)
	{
		coreList[i]->stop();
	}	
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Total time: "<< duration <<'\n';

	// when it returns from start, the token that resides in the inbox
	// will be the program result
	Token_Type *tok = core->inbox.front();
	cout<< "the result is : \n";
	cout<< tok->data.iValue;
	core->inbox.clear();

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
		while((*it) == '\t')
		{
			line.erase(it);
			if(it == line.end())
				break;
		}
		if(it == line.end())
			break;
	}
	return true;
}
