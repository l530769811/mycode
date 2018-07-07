#ifndef __SERVICEAPPMAIN_H__
#define __SERVICEAPPMAIN_H__

#ifndef WIN32_LEAN_AND_MEAN
#define  WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "winservice_define.h"
#include <UseCount.h>
#include <UseCount.cpp>
#include "MyMessagePool.h"

//extern int WinAppMain(unsigned long argc, TCHAR** argv);


class _WINSERVICE_IMPORT_EXPORT_DLL_ ServiceAppMain : public CMyMessagePool
{
public:
	ServiceAppMain(ServiceAppMain *pmain);
	~ServiceAppMain(void);


	friend int WinAppMain(unsigned long argc, TCHAR** argv);

	virtual int WinAppMain(unsigned long argc, TCHAR** argv) = 0;

};


#endif