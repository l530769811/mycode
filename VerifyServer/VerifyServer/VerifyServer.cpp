// VerifyServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Service.h"
#include "MyServiceAppMain.h"
#include "AppEntity.h"

CMyServiceAppMain mainApp(0);
CWinService init(&mainApp);

int _tmain(int argc, _TCHAR* argv[])
{
	CAppEntity appEnter(&mainApp);	
	CWinService::WinRunAsService();
	
#ifdef _DEBUG
	void *returnval = 0;
	CWinService::WaitServiceMain(&returnval);
#endif

	return 0;
}

