// VerifyServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Service.h"
#include "MyServiceAppMain.h"
#include "AppEntity.h"
#include "MyMessagePool.h"


const TCHAR		installService[] = 	_T("--install");
const TCHAR		unstallService[] =	_T("--unstall");
const TCHAR		runAsService[]	=  _T("--service");
const TCHAR		runAsUserApp[]  = _T("--run");

CMyServiceAppMain mainApp;
CWinService init(&mainApp);
CMyMessagePool message_pool(0);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMute = ::CreateMutex(0, TRUE, _T("VerifyServer_Mutex"));
	if (hMute!=INVALID_HANDLE_VALUE && ::GetLastError() != ERROR_ALIAS_EXISTS)
	{
		CAppEntity appEnter(&mainApp);	

		bool argfound = false;
		for (int i=1; i< argc; i++)
		{
			TCHAR *ch = argv[i];
			for (int j=0; j<lstrlen(ch); j++)
			{
				ch[j] = tolower(ch[j]);
			}

			for (int k=0; k< lstrlen(ch); k++)
			{
				if(ch[k] <= _T(' '))
				{
					continue;
				}
				argfound = true;
				size_t arglen = _tcscspn(&ch[k], _T("\t\r\n\v\f"));
				if (_tcsncmp(&ch[k], installService, arglen) == 0 &&
					arglen== lstrlen(installService))
				{
					CWinService::InstallService(_T("VerifyServer"), _T("VerifyServer"), _T("VerifyServer"), TRUE);
				}
				else if (_tcsncmp(&ch[k], unstallService, arglen) == 0 &&
					arglen== lstrlen(unstallService))
				{
					CWinService::RemoveService(_T("VerifyServer"), _T("VerifyServer"), TRUE);
				}
				else if (_tcsncmp(&ch[k], runAsService, arglen) == 0 &&
					arglen== lstrlen(runAsService))
				{
					CWinService::WinRunAsService();
				}
				else if (_tcsncmp(&ch[k], runAsUserApp, arglen) == 0 &&
					arglen== lstrlen(runAsUserApp))
				{
					void *returnval = 0;
					mainApp.WinAppMain(0, 0);
					CWinService::WaitServiceMain(&returnval);
					break;
				}
			}
		}

		if(argfound==false){
			void *returnval = 0;
			mainApp.WinAppMain(0, 0);
			CWinService::WaitServiceMain(&returnval);
		}

#ifdef _DEBUG
		void *returnval = 0;
		CWinService::WaitServiceMain(&returnval);
#endif

	}
	
	return 0;
}

