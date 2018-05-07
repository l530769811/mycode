#ifndef __ESOP_CAPPDATAFILECOPY_H__
#define __ESOP_CAPPDATAFILECOPY_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include "my_assist_define.h"

class CAppDataFileCopy
{
public:
	CAppDataFileCopy(const MyString &strProject, const MyString &strFileName);
	~CAppDataFileCopy(void);

public:
	MyString GetAppDataFileName();
	MyString GetAppDataRootPath();

protected:
	void _CopyAppData();
	void _CreateAddDirectory(const MyString &csAddPath);
private:
	TCHAR m_szAppDataPath[MAX_PATH];
	MyString m_strFileName;
	MyString m_strAppDataRootPath;
};

#endif //__ESOP_CAPPDATAFILECOPY_H__