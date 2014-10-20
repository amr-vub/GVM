/** 
* Tokenizer.h
* Amr Gamil
* GVM
*/

#pragma once

//#include <vector>
#include <map>
#include "Context.h"
#include "Core.h"
#include "PE.h"

using namespace std;

typedef vector<Token<int>> Vector_token;

class Switcher;

/*
* This class defines the Tokenizer.
* The tokenizer is responsible for:
 - collecting the result out of an execution of an execution packet.
 - Wrap this result in a token.
 - Send it to the token queue for furthure processing.
*/
class Tokenizer
{
public:
	Tokenizer(Core &core);
	Tokenizer(void);
	~Tokenizer(void);	
	// create a new token and send it to the token queue
	void wrapAndSend(Tuple_vector &distList, int &res, Context &cx);

	/*	fields	*/
	Core core;

	Switcher *swicther;
};

/*
	class to handel the switch instruction special functionality
*/
class Switcher
{
public:
	Switcher();
	Switcher(Tokenizer &tokenizer);
	~Switcher();

	// add an element to the map
	void addSwitchStorageElement(Token<int> &tok);

	Vector_token getAllElement(long &cx);

	// a storage map for switch instruction tokens	
	map<long, Vector_token> switchStorage;

	// Forward tokens to the tokenizer queue
	void sendToTokinzer(Tuple_vector &dest, Vector_token &tokV);

	/*	fields	*/
	Tokenizer tokenizer;
};