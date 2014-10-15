/** 
* Tokenizer.h
* Amr Gamil
* GVM
*/

#pragma once

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
	Tokenizer(void);
	~Tokenizer(void);	
};

