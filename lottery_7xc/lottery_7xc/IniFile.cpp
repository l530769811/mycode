// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "IniFile.h"
#include <io.h>
#include <Windows.h>
#include <tchar.h>
#include <cstdlib>
#include <sstream>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile(std::wstring  csFile) : m_csFile(csFile)
{
}

CIniFile::~CIniFile()
{
}

std::wstring  CIniFile::ReadString(const std::wstring & csSection, const std::wstring & csItem,
	const std::wstring & csDefault)
{
	TCHAR szBuffer[1025] = {0};
	DWORD err = ::GetPrivateProfileString(csSection.c_str(), csItem.c_str(), csDefault.c_str(), szBuffer, 1024,
		m_csFile.c_str());
#ifdef _DEBUG
	DWORD codeerr = ::GetLastError();
#endif
	
	return szBuffer;
}

bool CIniFile::ReadBool(const std::wstring & csSection, const std::wstring & csItem,
	bool bDefault)
{
	UINT nRet;
	nRet = GetPrivateProfileInt(csSection.c_str(), csItem.c_str(), (bDefault) ? 1 : 0,
			m_csFile.c_str());
	return (nRet == 1);
}

UINT CIniFile::ReadInt(const std::wstring & csSection, const std::wstring & csItem,
	UINT iDefault)
{
	UINT nRet;
	nRet = GetPrivateProfileInt(csSection.c_str(), csItem.c_str(), iDefault, m_csFile.c_str());
	return nRet;
}

double CIniFile::ReadDouble(const std::wstring & csSection, const std::wstring & csItem,
	double dDefault)
{
	TCHAR szBuffer[25];
	TCHAR szValue[25];
	wsprintf(szValue, _T("%f"), dDefault);
	GetPrivateProfileString(csSection.c_str(), csItem.c_str(), szValue, szBuffer, 24, m_csFile.c_str());
	return _ttoi((szBuffer));
}

void CIniFile::WriteString(const std::wstring & csSection, const std::wstring & csItem,
	const std::wstring & csValue)
{
	WritePrivateProfileString(csSection.c_str(), csItem.c_str(), csValue.c_str(), m_csFile.c_str());
}

void CIniFile::WriteBool(const std::wstring & csSection, const std::wstring & csItem,
	bool  bValue)
{
	WriteString(csSection, csItem, (bValue) ? _T("1") : _T("0"));
}

void CIniFile::WriteInt(const std::wstring & csSection, const std::wstring & csItem,
	UINT iValue)
{
	std::wstring  csValue;
	//csValue._T("%d"), iValue);
	::itoa(iValue, (char*)csValue.c_str(), 10);
	WriteString(csSection, csItem, csValue);
}

void CIniFile::WriteFloat(const std::wstring & csSection, const std::wstring & csItem,
	double fValue)
{
	std::wstring  csValue;
	//csValue.Format(_T("%f"), fValue);

	std::wostringstream sstream;
	sstream << fValue;
	csValue = sstream.str();

	WriteString(csSection.c_str(), csItem.c_str(), csValue.c_str());
}

bool CIniFile::IsExistFile()
{
	if( (_taccess( m_csFile.c_str(), 0 )) != -1 )
		return TRUE;
	else
		return FALSE;
}

bool CIniFile::HasWritePermission()
{
	if( (_taccess( m_csFile.c_str(), 0 )) != -1 )
    {
        if( (_taccess(_T("crt_ACCESS.C"), 2 )) != -1 )
            return TRUE;
    }
	return FALSE;
}