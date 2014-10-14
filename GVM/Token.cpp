/** 
* Token.cpp
* Amr Gamil
* GVM
*/
#include "stdafx.h"
#include "Token.h"

template <class T>
Token<T>::Token(T *dataInput, Tag tagInput)
{
	data = dataInput;
	tag = tagInput;
}

template <class T>
Token<T>::~Token(void)
{
	delete data;
}

// copy constructor
template <class T>
Token<T>::Token(const Token &obj)
{
	T *dat = new T;
	dat = *obj.data;
}

// setters and getters for the class fields

template <class T>
void Token<T>::setData(T *dat)
{
	data = dat;
}

template <class T>
void Token<T>::setTag(Tag tg)
{
	tag = tg;
}

template <class T>
T* Token<T>::getData(void)
{
	return data;
}

template <class T>
Tag Token<T>::getTag(void)
{
	return tag;
}