#include "ServiceLogFile.h"
#include <Shlwapi.h>
#include <Shlobj.h>

CServiceLogFile::CServiceLogFile(mystring csFileName)
	: m_csFileName(csFileName)
{

}


CServiceLogFile::CServiceLogFile()
{
	mychar str[MAX_PATH] = {0};
	mystring csPath, csValue;
	::GetModuleFileName(NULL, str, MAX_PATH);
	csPath = str;
	mystring csFileName = ::PathFindFileName(csPath.c_str());
	int nPosExe = csFileName.find(_T('.'));
	csFileName = csFileName.substr(0, nPosExe);
	int nPos = -1;
	nPos = csPath.rfind(_T('\\'));
	csPath = csPath.substr(0, nPos);
	csPath = csPath + _T('\\') + LOG_FILE_FOLDER;
	if (csFileName.empty() == false)
	{
		csPath = csPath + _T('\\') + csFileName;
	}
	if (_taccess(csPath.c_str(), 0) == -1)
	{
		::SHCreateDirectoryEx(NULL, csPath.c_str(), NULL);
	}
	

	csPath = csPath + _T('\\') + LOG_FILE_NAME;


	m_csFileName = csPath;
}

CServiceLogFile::~CServiceLogFile(void)
{
}

int CServiceLogFile::WriteLog(const mystring csLog)
{
	int nret = -1;
	OFSTREAM file(m_csFileName, std::ios::app);
	mystring str_log(csLog);
	if (file.is_open())
	{
		time_t cur_time = 0;
		cur_time = time(0);
		struct tm *local;
		local = localtime(&cur_time);
		mychar pChTime[40] = {0};
		STRFTIME(pChTime, 40, _T("[%Y-%m-%d %H:%M:%S]"), local);
		str_log = _T('\t') + str_log;
		str_log = pChTime + str_log;
		
		//file.seekp(0, std::ios::end);
		//file.tellp();
		file << str_log;
		file << _T("\r\n");
		file << _T("\r\n");

		file.close();
		nret = lstrlen(str_log.c_str());
	}

	return nret;
}

int CServiceLogFile::IntervalWriteLog(const mystring csLog, time_t nIntervalTime)
{
	int nret = -1;

	static time_t nLastTime = 0;
	OFSTREAM file(m_csFileName, std::ios::app);
	if (file.is_open())
	{
		mystring str_log(csLog);
		time_t cur_time = 0;
		cur_time = time(0);
		struct tm *local;
		local = localtime(&cur_time);

		mychar pChTime[40] = {0};
		STRFTIME(pChTime, 40, _T("[%Y-%m-%d %H:%M:%S]"), local);
		str_log = _T('\t') + str_log;
		str_log = pChTime + str_log;
		if (cur_time - nLastTime >= nIntervalTime)
		{		
			file << str_log;
			file << _T("\r\n");
			file << _T("\r\n");
			nLastTime = cur_time;
		}
		
		file.close();
		nret = lstrlen(str_log.c_str());
	}

	return nret;
}

int CServiceLogFile::_WriteLog(const mychar *pszFmt,va_list argp)
{
	int nret = -1;
	if (NULL==pszFmt||0==pszFmt[0]) return nret;
	mychar logstr[MAX_PATH] = {0};
	VSNPRINTF(logstr,MAX_PATH,pszFmt,argp);
	mystring csLog = logstr;
	OFSTREAM file(m_csFileName, std::ios::app);
	if (file.is_open())
	{
		time_t cur_time = 0;
		cur_time = time(0);
		struct tm *local;
		local = localtime(&cur_time);
		mychar pChTime[40] = {0};
		STRFTIME(pChTime, 40, _T("[%Y-%m-%d %H:%M:%S] "), local);
		csLog = _T('\t') + csLog;
		csLog = pChTime + csLog;

		//file.seekp(0, std::ios::end);
		//file.tellp();
		file << csLog;
		file << _T("\r\n");
		file << _T("\r\n");

		file.close();
		nret = lstrlen(csLog.c_str());
	}

	return nret;
}

int CServiceLogFile::WriteLog(const mychar *pszFmt,...)
{
	int nret = -1;

	va_list argp;
	va_start(argp,pszFmt);
	nret = _WriteLog(pszFmt,argp);
	va_end(argp);

	return nret;
}