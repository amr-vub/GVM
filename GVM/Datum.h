/** 
* Datum.h
* Amr Gamil
* GVM
*/

#pragma once
#include <string>
#include <vector>

using namespace std;

/*
	This class provide all data types that the VM supports
*/
class Datum
{
public:
	enum Type{
		INT,
		FLOAT,
		CHAR,
		BOOLEAN,
		STRING,
		I_VECTOR,
		F_VECTOR,
		B_VECTOR
	} token_Type;

	Datum(int iValue = NULL,float fValue = NULL, char cValue = NULL, const string& sValue = " ");
	~Datum(void);

	//changing the type
	void setTo(Type eType);
	// overloading primitive operators
	/*	+	*/
	Datum operator+(const Datum& b);
	/*	==	*/
	Datum operator==(const Datum& b);
	/*	-	*/
	Datum operator-(const Datum& b);
	/*	*	*/
	Datum operator*(const Datum& b);
	/*	/	*/
	Datum operator/(const Datum& b);
	/*	>	*/
	Datum operator>(const Datum& b);
	/*	<	*/
	Datum operator<(const Datum& b);

	// function to abstract types from the split instruction
	//static vector<Datum> returnVectorOfDatums(Datum &dat);

	/*	Fields	*/
	int iValue;
	float fValue;
	char cValue;
	bool bValue;
	string sValue;
	vector<int> iValue_v;
	vector<float> fValue_v;
	vector<bool> bValue_v;
};

