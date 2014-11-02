/** 
* Tokenizer.h
* Amr Gamil
* GVM
*/

#pragma once

#include <tuple>
#include <map>
#include "Token_Type.h"
#include "Context.h"
#include "PE.h"

using namespace std;

class Core;
class Tokenizer;

struct RestoreArgs;
typedef vector<Token_Type*> Vector_token;
typedef map<long, RestoreArgs> RestorMapType;
typedef map<long, tuple<short, Context*>> ContextMapType;



// data structure to hold the restored info
struct RestoreArgs{	
	// det chunk 
	short chunk;
	// dest instruction address
	int idx;
	// dest port number
	short port;
	// old context
	Context *cx;
	// number of expected return values
	short restores;
};

/*
	class to handel the switch instruction special functionality
*/
class Switcher
{
public:
	Switcher();
	//Switcher(Tokenizer &tokenizer);
	~Switcher();

	// add an element to the map
	void addSwitchStorageElement(Token_Type *tok);

	Vector_token getAllElement(long &cx);

	// a storage map for switch instruction tokens	
	map<long, Vector_token> switchStorage;

	// Forward tokens to the tokenizer queue
	void sendToTokinzer(Tuple_vector &dest, Vector_token &tokV);

	/*	fields	*/
	Tokenizer *tokenizer;
};

/*
 Class ContextManager That manages the context change and restore
*/
class ContextManager
{
public:
	//ContextManager(Tokenizer &tokenizer);
	ContextManager();
	~ContextManager();

	// 
	void bind_save(Token_Type &tok, int* destAdd, int* retAdd, short &binds, short &rest);

	// restore the cx for the recieved tok
	void restore(Token_Type &tok);

	/*	fields	*/

	// A map to store the return address and the old context for
	// the new created context. The key will be the old context
	RestorMapType restoreMap; 

	// A map to store the new created context in case of multiple <binds>
	// i.e. when we have to send multiple arguments to the same function
	// The key here is the Old conxId
	ContextMapType contextMap;

	Tokenizer *tokenizer;

private:
	void bind_send(Token_Type &tok, int* destAdd, int* retAdd, short &rest, Context* cx);	
};


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
	Tokenizer();
	~Tokenizer(void);	
	// create a new token and send it to the token queue
	void wrapAndSend(Tuple_vector &distList, Datum &res, Context &cx);

	//
	void sendStop(Token_Type *tok);

	/*	fields	*/
	Core *core;

	Switcher swicther;
	ContextManager contextManager;
};
