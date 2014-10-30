/** 
* Token.h
* Amr Gamil
* GVM
*/

/**
* This file describes Token, which is the element that
* encapsulate all of the information required for
* execution
*/
#pragma once
#include "Tag.h"
/** 
* Represents a GVM Token
* Token has data field and a tag field
* - data encapsulate values
* - tag identify the Token
*/
template<class T>
class Token
{
	public:
		Token(T &dat, Tag *tg);
		Token(void);
		~Token(void);		

		T data;
		Tag *tag;
};

template <class T>
Token<T>::Token(T &dataInput, Tag *tagInput)
{
	data = dataInput;
	tag = tagInput;
}

template <class T>
Token<T>::Token(void)
{

}

template <class T>
Token<T>::~Token(void)
{
	delete tag;
}

