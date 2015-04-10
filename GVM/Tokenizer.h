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
typedef unordered_map<long, RestoreArgs> RestorMapType;
typedef map<pair<long, long>, tuple<short, Context*>> ContextMapType;



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

	Vector_token getAllElement(long &cx, long &instAdd);

	// store the selected dest recieved by tokens with port == 0,
	// in case there still exist tokens to come
	void storeDest(Token_Type *tok, short remainingInputs, int* indx);

	// update and do garbage collection if needed in the savedDestinations
	tuple<short, int*> updateStoredDest(Token_Type *tok);

	// check if port 0 token has already been recieved 
	bool alreadyExists(Token_Type *tok);

	// a storage map for switch instruction tokens	
	map<pair<long,long>, Vector_token> switchStorage;

	// s storage that hold selected dest recieved by tokens with port == 0
	// to be saved for further tokens with same pair<cx,InsAdd>
	map<pair<long,long>, tuple<short,int*>> savedDestinations;

	// Forward tokens to the tokenizer queue
	void sendToTokinzer(Vector_Tuple &dest, Vector_token &tokV);

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
	void bind_save(Token_Type &tok, int* destAdd, int* retAdd, short &binds, short rest, 
		long &instIdx, vector<tuple<short, Datum>> literals);

	void bind_send(Token_Type &tok, int* destAdd, short destPort
		, short sentPort, int* retAdd, short rest, Context* cx);	

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
	void wrapAndSend(Vector_Tuple &distList, Datum &res, Context &cx);

	//
	void sendStop(Token_Type *tok);

	/*	fields	*/
	Core *core;

	Switcher swicther;
	ContextManager contextManager;
};
