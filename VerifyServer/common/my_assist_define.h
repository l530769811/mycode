#ifndef _COMMON_MY_ASSIST_H__
#define _COMMON_MY_ASSIST_H__


#include <string>
#ifdef _UNICODE
#define MyString std::wstring
#else
#define MyString std::string
#endif

#ifdef _UNICODE

#define mychar   TCHAR
#define mystrftime  wcsftime
#define mysnprintf       _vsnwprintf
#define myofstream	std::wofstream

#else

#define mychar  char
#define  mystrftime  strftime
#define   mysnprintf       vsnprintf
#define myofstream	std::ofstream

#endif

#define IS_NULL(arg) if((arg)==NULL)
#define NO_NULL(arg) if((arg)!=NULL)

#define SAFE_DELETE(arg) \
	if((arg)!=NULL) \
	{ \
	delete (arg); \
	(arg) = NULL; \
	}

#endif