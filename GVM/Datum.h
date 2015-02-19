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
		ULONG_LONG,
		FLOAT,
		CHAR,
		BOOLEAN,
		STRING,
		I_VECTOR,
		F_VECTOR,
		B_VECTOR
	} token_Type;

	Datum( unsigned long long uLValue = 0, int iValue = -1, float fValue = -1, char cValue = -1, const string& sValue = " ");	
	~Datum();

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
	/*	>=	*/
	Datum operator>=(const Datum& b);
	/*	<	*/
	Datum operator<(const Datum& b);

	// function to abstract types from the split instruction
	//static vector<Datum> returnVectorOfDatums(Datum &dat);

	/*	Fields	*/
	int iValue;
	unsigned long long uLValue;
	float fValue;
	char cValue;
	bool bValue;
	string sValue;
	vector<int> iValue_v;
	vector<float> fValue_v;
	vector<bool> bValue_v;
};

