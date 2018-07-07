#include "LogCollecter.h"
#include <tchar.h>
#include <iostream>



void DoException(int limite, std::exception &e)
{
	if(CLogCollecter::m_pThis!=0)
	{
		
		MyString strLog;
		strLog.resize(strlen(e.what())+1);
		mbstowcs((TCHAR*)strLog.c_str(), e.what(), strlen(e.what()));
		CLogCollecter::m_pThis->Log(limite, strLog);
	}
}

int write_debug_log(MyString strLog)
{
	int nret = -1;

#ifdef _DEBUG

#ifdef _UNICODE
	std::wcout << strLog <<std::endl;
#else
	std::cout << strLog <<std::endl;
#endif

#endif // _DEBUG

	return nret;
}
CLogCollecter*	CLogCollecter::m_pThis = 0;
CLogCollecter::CLogCollecter(CLogCollecter *log_collecter)
{
	memset(m_arr_log_func, 0, sizeof(WRITE_LOG)*sizeof(unsigned short)*8);
	CLogCollecter::m_pThis = log_collecter;
	this->AddLimite(DEBUG_LOG, write_debug_log);

}


CLogCollecter::~CLogCollecter(void)
{
}

int CLogCollecter::_get_index(int limite)
{
	int nret = -1;
	if (limite<LOG_MAX_INDEX && limite>LOG_MIN_INDEX)
	{
		int nindex = 0;
		while ( (limite>>=1)>0 )
		{
			nindex++;
		}
		nret = nindex;
	}

	return nret;
}

bool CLogCollecter::_add_long_func(int nlimite, WRITE_LOG func)
{
	bool bret = false;
	if (nlimite<LOG_MAX_INDEX && nlimite>LOG_MIN_INDEX)
	{
		int nindex = 0;
		nindex = _get_index(nlimite);
		m_arr_log_func[nindex]=func;
		bret = true;
	}
	return bret;
}

bool CLogCollecter::AddLimite(int nlimite, WRITE_LOG func)
{
	bool bret = false;
	if (nlimite<LOG_MAX_INDEX && nlimite>LOG_MIN_INDEX)
	{
		m_log_type_bit = m_log_type_bit | nlimite;
		bret = _add_long_func(nlimite, func);
	}

	return bret;
}

bool CLogCollecter::RemoveLimite(int nlimite)
{
	bool bret = false;
	if (nlimite<LOG_MAX_INDEX && nlimite>LOG_MIN_INDEX)
	{
		
		m_log_type_bit = m_log_type_bit & (~nlimite);
		int nindex = _get_index(nlimite);
	}
	return bret;
}

bool CLogCollecter::CanLimite(int nlimite)
{
	bool bret = false;
	if (nlimite<LOG_MAX_INDEX && nlimite>LOG_MIN_INDEX)
	{
		if(m_log_type_bit & nlimite != nlimite || m_arr_log_func[_get_index(nlimite)]==0)
		{
			bret = true;
		}
	}

	return bret;
}

int CLogCollecter::Log(int limite, MyString str_log_text)
{
	if ( (m_log_type_bit&limite) == limite )
	{
		int n = _get_index(limite);
		int nmax = sizeof(m_log_type_bit)*8;
		if (n>=0 && n<nmax && m_arr_log_func[n]!=0)
		{
			return m_arr_log_func[n](str_log_text);
		}
	}

	return -1;
}