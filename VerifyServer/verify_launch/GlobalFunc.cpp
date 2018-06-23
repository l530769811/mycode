#include <WinSock2.h>
#include <MSWSock.h>
#include <MSTcpIP.h>
#include "StdAfx.h"
#include "GlobalFunc.h"
#include "Shlwapi.h"
#include <stdio.h>

MyString GetModuleFilePath()
{
	TCHAR str[MAX_PATH] = {0};
	MyString csBasePath;
	::GetModuleFileName(NULL, str, MAX_PATH);
	csBasePath = str;

	int nPos = -1;
	nPos = csBasePath.rfind(_T('\\'));
	csBasePath = csBasePath.substr(0, nPos);
	return csBasePath;
}

int GlobalUnicodeToUtf8(const TCHAR *unicode, char *uft8, int len)
{
	int utf8size = 0;
#ifdef _UNICODE
	utf8size = ::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	if (utf8size > len)
	{
		return NULL;
	}
	

	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)(LPCWSTR)unicode, -1, uft8, sizeof(char *)*utf8size, NULL, NULL);
#else
	strcpy(uft8, unicode);
	utf8size = len;
#endif //_UNICODE
	return utf8size;
}

TCHAR* GlobalUtf8ToUnicode(const char *uft8, TCHAR *unicode, int len)
{
#ifdef _UNICODE
	
	int nLen = 0;
	nLen = MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, NULL, 0);
	if (len < nLen)
	{
		return NULL;
	}
	::memset(unicode, 0, len * sizeof(TCHAR));
	::MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, unicode, nLen);

#else
	strcpy(unicode, uft8);
#endif //_UNICODE
	return unicode;
}