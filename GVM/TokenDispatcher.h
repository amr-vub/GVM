/** 
* TokenDispatcher.h
* Amr Gamil
* GVM
*/
#pragma once

/*
* This class represents the first element in the GVM pipleine,
* the TokenDispatcher, which is responsible for dispatching tokens
* from the token queue to the next step in the GVM pipeline, the
* context matcher.
* TODO: think about rmoving it!
*/
class TokenDispatcher
{

public:
	TokenDispatcher(void);
	~TokenDispatcher(void);
};

