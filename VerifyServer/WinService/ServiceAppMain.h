#ifndef __SERVICEAPPMAIN_H__
#define __SERVICEAPPMAIN_H__

#ifndef WIN32_LEAN_AND_MEAN
#define  WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "winservice_define.h"


class _WINSERVICE_IMPORT_EXPORT_DLL_ ServiceAppMain 
{
public:
	ServiceAppMain(void);
	~ServiceAppMain(void);

public:
	virtual int WinAppMain(unsigned long argc, TCHAR** argv) = 0;

};


#endif