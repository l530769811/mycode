#include "MyServiceAppMain.h"


CMyServiceAppMain::CMyServiceAppMain(ProcCallBack call_back)
	: ServiceAppMain(this, call_back)
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
