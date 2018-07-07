#include "MyServiceAppMain.h"
#include "processlogcollecter.h"
#include <tchar.h>

CMyServiceAppMain::CMyServiceAppMain()
	: ServiceAppMain(this)
{
}


CMyServiceAppMain::~CMyServiceAppMain(void)
{
}

int CMyServiceAppMain::WinAppMain(unsigned long argc, TCHAR** argv)
{
	log(FILE_LOG, _T(" cmyserviceappmain::winappmain begin"));
	int ret = 0;
	MyMSG msg;
	while (MyGetMessage(&msg))
	{
		MyDispatchMessage(&msg);
	}
	log(FILE_LOG, _T(" cmyserviceappmain::winappmain end"));
	return ret;
}
