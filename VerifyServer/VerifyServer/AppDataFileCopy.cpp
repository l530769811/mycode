#include "StdAfx.h"
#include "AppDataFileCopy.h"
#include "GlobalFunc.h"
#include <cassert>
#include <Shlobj.h>
#include "Shlwapi.h"
#include "ProcessLogCollecter.h"
#pragma comment(lib, "Shlwapi.lib")

CAppDataFileCopy::CAppDataFileCopy(const MyString &strProject, const MyString &strFileName)
	: m_strFileName(strFileName)
{
	memset(m_szAppDataPath, 0, MAX_PATH);
	//获取系统用户数据保存根路径;
	::SHGetSpecialFolderPath(NULL,m_szAppDataPath,CSIDL_APPDATA,FALSE);
	_CreateAddDirectory(strProject);
	_CopyAppData();
}


CAppDataFileCopy::~CAppDataFileCopy(void)
{
}

void CAppDataFileCopy::_CreateAddDirectory(const MyString &csAddPath)
{
	::PathAppend(m_szAppDataPath, csAddPath.c_str());

	//路径是否存在，不存在则创建;
	if (!::PathFileExists(m_szAppDataPath))
	{
		::CreateDirectory(m_szAppDataPath, NULL);
	}
	m_strAppDataRootPath = m_szAppDataPath;
}

void CAppDataFileCopy::_CopyAppData()
{
	::PathAppend(m_szAppDataPath, m_strFileName.c_str());
	MyString strFilePath(m_szAppDataPath);
	BOOL bSpec = ::PathRemoveFileSpec( const_cast<TCHAR*>(strFilePath.c_str()) );

	if (::PathFileExists(strFilePath.c_str()) == FALSE)
	{
		BOOL bCreate = CreateDirectoryMulti(const_cast<TCHAR*>(strFilePath.c_str()));
		if (bCreate==FALSE)
		{
			MyString str = __FILEW__; 
			log(FILE_LOG,  str+_T("  CreateDirectoryMulti fail"));
		}
		assert(bCreate);
	}
	else
	{
		MyString str = __FILEW__; 
		str = str + _T(" ") + strFilePath + _T(" exist");
		log(FILE_LOG,  str);
	}

	if (::PathFileExists(m_szAppDataPath) == FALSE)
	{
		MyString strExePath = ::GetModuleFilePath();
		TCHAR str[MAX_PATH]  = {0};
		_tcscpy(str, strExePath.c_str());
		::PathAppend(str, m_strFileName.c_str());
		BOOL bCopy = ::CopyFile(str, m_szAppDataPath, TRUE);
		if (bCopy==FALSE)
		{
			MyString str = __FILEW__; 
			log(FILE_LOG,  str+_T("  CopyFile fail"));
		}
		assert(bCopy);
	}
	else
	{
		MyString str = __FILEW__; 
		str = str + _T(" ") + m_szAppDataPath + _T(" exist");
		log(FILE_LOG,  str);
	}
}

MyString CAppDataFileCopy::GetAppDataFileName()
{
	return m_szAppDataPath;
}

MyString CAppDataFileCopy::GetAppDataRootPath()
{
	return m_strAppDataRootPath;
}