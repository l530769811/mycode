#include "ServiceAppMain.h"


int WinAppMain(unsigned long argc, TCHAR** argv)
{
	//ServiceAppMain h(0);
	//MyMSG msg;
	//while (h.MyGetMessage(&msg))
	//{
	//	h.MyDispatchMessage(&msg);
	//}

	if(ServiceAppMain::m_pmain!=0)
	{
		ServiceAppMain::m_pmain->WinAppMain(argc, argv);
	}
	return 0;
}

ServiceAppMain* ServiceAppMain::m_pmain = 0;

ServiceAppMain::ServiceAppMain(ServiceAppMain *pmain)
{
	
	ServiceAppMain::m_pmain = pmain;
}


ServiceAppMain::~ServiceAppMain(void)
{
	
}