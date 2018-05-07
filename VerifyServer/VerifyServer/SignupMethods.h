#ifndef _SIGNUP_METHODS_H__
#define _SIGNUP_METHODS_H__

class CSignupMethods
{
public:
	CSignupMethods(void);
	virtual ~CSignupMethods(void);

public:
	virtual bool signupMethodVerify() = 0; 
};

#endif //_SIGNUP_METHODS_H__