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
		Token(T *dat, Tag tg);
		~Token(void);
		Token (const Token &obj);
		T *data;
		Tag tag;
		/*
		void setData(T *dat);
		void setTag(Tag tg);
		Tag getTag(void);
		T* getData(void);
		*/
};

