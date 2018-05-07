#include "StdAfx.h"
#include "DBProxy.h"

#include <Windows.h>


//char* UnicodeToUtf8(const TCHAR *unicode, char *uft8, int len)
//{
//#ifdef _UNICODE
//	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)(LPCWSTR)unicode, -1, uft8, sizeof(char *)*len, NULL, NULL);
//#else
//	strcpy(unicode, uft8);
//#endif //_UNICODE
//	return uft8;
//}
//
//TCHAR* Utf8ToUnicode(const char *uft8, TCHAR *unicode, int len)
//{
//#ifdef _UNICODE
//	int nLen = 0;
//	nLen = MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, NULL, 0);
//	if (len <= nLen)
//	{
//		return NULL;
//	}
//	::MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, unicode, nLen);
//
//#else
//	strcpy(utf8, unicode);
//#endif //_UNICODE
//	return unicode;
//}

CDBProxy::CDBProxy(void)
{
	
}


CDBProxy::~CDBProxy(void)
{

}



