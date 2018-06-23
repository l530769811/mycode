#include "MyServiceAppMain.h"


CMyServiceAppMain::CMyServiceAppMain()
	: ServiceAppMain(this)
{
}


CMyServiceAppMain::~CMyServiceAppMain(void)
{
}

int CMyServiceAppMain::WinAppMain(unsigned long argc, TCHAR** argv)
{
	int ret = 0;
	MyMSG msg;
	while (MyGetMessage(&msg))
	{
		MyDispatchMessage(&msg);
	}
	return ret;
}
