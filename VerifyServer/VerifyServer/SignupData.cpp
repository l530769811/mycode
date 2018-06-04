#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include "SignupData.h"
#include "sqlite_sql.h"

CClientSignupData::CClientSignupData(MyString strUserName,
	MyString strUserPassword,
	MyString strPhone)
	: m_strUserName(strUserName)
	, m_strUserPassword(strUserPassword)
	, m_strPhone(strPhone)
{
}


CClientSignupData::~CClientSignupData(void)
{
}

MyString CClientSignupData::ToSql() const
{
	TCHAR sql[1024] = {0};
	_stprintf(sql, insert_clientuser_data, m_strUserName.c_str(), m_strUserPassword.c_str(), m_strPhone.c_str(), _T(""), _T(""));
	return MyString(sql);
}
