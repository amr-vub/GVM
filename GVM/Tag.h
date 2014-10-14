/** 
* Tag.h
* Amr Gamil
* GVM
*/
#pragma once
/**
* Tag is the token identifier, it has three fields
* \param 
		contx: the context of the token (dynamic, can be changed during runtime)
* \param
		port: which port of the destination instruction 
		should the token be forwarded
* \param
		inst: the destination instruction address
*/
class Tag
{
private:
	int conx;
	short port;
	int inst;

public:
	Tag(int cx, short port, int ins);
	~Tag(void);
	void setConx(int cx);
	void setPort(short prt);
	void setInst(int ins);
	int getConx(void);
	short getPort(void);
	int getInst(void);
};

