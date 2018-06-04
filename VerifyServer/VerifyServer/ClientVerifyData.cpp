#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include "ClientVerifyData.h"
#include "sqlite_sql.h"

CClientVerifyData::CClientVerifyData(const MyString &strUserName,
	const MyString &strUserPassword )
	: m_strUserName(strUserName)
	, m_strUserPassword(strUserPassword)
{
}


CClientVerifyData::~CClientVerifyData(void)
{
}

MyString CClientVerifyData::ToSql() const
{
	TCHAR sql[MAX_PATH] = {0};
	_stprintf(sql, select_Login, m_strUserName.c_str(), m_strUserPassword.c_str());
	return MyString(sql);
}