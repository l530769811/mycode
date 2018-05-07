#ifndef __SERVICEAPPMAIN_H__
#define __SERVICEAPPMAIN_H__

#ifndef WIN32_LEAN_AND_MEAN
#define  WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "TcpServer.h"


extern int WinAppMain(DWORD argc, TCHAR** argv);


class ServiceAppMain
{
public:
	ServiceAppMain(void);
	~ServiceAppMain(void);


};


#endif
