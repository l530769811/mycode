#ifndef _SIGNUPDATA_H__
#define _SIGNUPDATA_H__
#include "my_assist_define.h"

class CClientSignupData
{
public:
	CClientSignupData(MyString m_strUserName,
	MyString m_strUserPassword,
	MyString m_strPhone);
	virtual ~CClientSignupData(void);

public:
	MyString ToSql() const;

private:
	MyString m_strUserName;
	MyString m_strUserPassword;
	MyString m_strPhone;
};

#endif //_SIGNUPDATA_H__