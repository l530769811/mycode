#include "ServiceAppMain.h"

static ServiceAppMain* pmain = 0;

int WinAppMain(unsigned long argc, TCHAR** argv)
{
	//ServiceAppMain h(0);
	//MyMSG msg;
	//while (h.MyGetMessage(&msg))
	//{
	//	h.MyDispatchMessage(&msg);
	//}

	if(pmain!=0)
	{
		pmain->WinAppMain(argc, argv);
	}
	return 0;
}



ServiceAppMain::ServiceAppMain(ServiceAppMain *main)
{	
	pmain = main;
}


ServiceAppMain::~ServiceAppMain(void)
{
	
}