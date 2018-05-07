#ifndef _CLIENTVERIFYDATA_H__
#define _CLIENTVERIFYDATA_H__

#include <my_assist_define.h>
class CClientVerifyData
{
public:
	CClientVerifyData(const MyString &strUserName,
		const MyString &strUserPassword);
	~CClientVerifyData(void);

public:
	MyString ToSql() const;

private:
	MyString m_strUserName;
	MyString m_strUserPassword;
};

#endif