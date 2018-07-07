#ifndef  __VERIFYSERVER_LOGFILE_H__
#define  __VERIFYSERVER_LOGFILE_H__

#ifndef __LOGFILE_INCLUDE_WINDOWS_HEADER_H__
#define __LOGFILE_INCLUDE_WINDOWS_HEADER_H__
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif //__LOGFILE_INCLUDE_WINDOWS_HEADER_H__

#include <string>
using namespace std;

#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <stdarg.h>
#include <tchar.h>

#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Shell32.lib")

//#ifndef _DEFILE_STD_MYSTRING
//#define _DEFILE_STD_MYSTRING

#ifndef _UNICODE
//#define  mystring std:string
#define  mystring string
#else
//#define  mystring std:wstring
#define  mystring wstring
#endif //_UNICODE
//#endif //_DEFILE_STD_MYSTRING

#ifdef _UNICODE

#define mychar   TCHAR
#define STRFTIME  wcsftime
#define VSNPRINTF       _vsnwprintf
#define OFSTREAM	std::wofstream

#else

#define mychar  char
#define  STRFTIME  strftime
#define   VSNPRINTF       vsnprintf
#define OFSTREAM	std::ofstream
			
#endif


#define LOG_FILE_NAME _T("log.txt")
#define LOG_FILE_FOLDER _T("log")
class CLogFile
{
public:
	CLogFile(mystring csFileName);
	CLogFile();
	~CLogFile(void);

	int WriteLog(const mystring csLog);
	int WriteLog(const mychar *pszFmt,...);
	int IntervalWriteLog(const mystring csLog, time_t nIntervalTime);//间隔nIntervalTime(毫秒)时间写入日记;
private:
	int _WriteLog(const mychar *pszFmt,va_list argp);

private:
	mystring m_csFileName;
};

#endif // end __VERIFYSERVER_LOGFILE_H__
