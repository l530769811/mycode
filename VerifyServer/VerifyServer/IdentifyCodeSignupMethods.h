#pragma once
#include "signupmethods.h"
class CIdentifyCodeSignupMethods :
	public CSignupMethods
{
public:
	CIdentifyCodeSignupMethods(void);
	virtual ~CIdentifyCodeSignupMethods(void);

public:
	virtual bool signupMethodVerify();
};

