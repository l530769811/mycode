// VerifyServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Service.h"
#include "MyServiceAppMain.h"
#include "AppEntity.h"
#include "MyMessagePool.h"
#include "ProcessLogCollecter.h"


const TCHAR		installService[] = 	_T("--install");
const TCHAR		unstallService[] =	_T("--unstall");
const TCHAR		runAsUserApp[]  = _T("--run");
const TCHAR runAsService[]		= _T("--service");

#define INSTALL		(1<<0)
#define UNSTALL		(1<<1)
#define SERVICE		(1<<2)
#define ASUSER		(1<<3)

CMyServiceAppMain mainApp;
CWinService init(&mainApp);

int _tmain(int argc, _TCHAR* argv[])
{
	log(FILE_LOG, _T("process begin"));
	HANDLE hMute = ::CreateMutex(0, TRUE, _T("VerifyServer_Mutex"));
	//if (hMute==INVALID_HANDLE_VALUE || ::GetLastError() == ERROR_ALREADY_EXISTS)
	//{
	//	return 0;
	//}
	unsigned short arg_bit = 0;
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
#ifndef _DEBUG
				_tprintf(_T("===========%s\n"), installService);
#endif
				arg_bit = arg_bit | INSTALL;
				break;
			}
			else if (_tcsncmp(&ch[k], unstallService, arglen) == 0 &&
				arglen== lstrlen(unstallService))
			{
#ifndef _DEBUG
				_tprintf(_T("===========%s\n"), unstallService);
#endif
				arg_bit = arg_bit | UNSTALL;
			
				break;
			}
			else if (_tcsncmp(&ch[k], runAsService, arglen) == 0 &&
				arglen== lstrlen(runAsService))
			{
#ifndef _DEBUG
				_tprintf(_T("===========%s\n"), runAsService);
#endif
				arg_bit = arg_bit | SERVICE;
				
				break;

			}
			else if (_tcsncmp(&ch[k], runAsUserApp, arglen) == 0 &&
				arglen== lstrlen(runAsUserApp))
			{
#ifndef _DEBUG
				_tprintf(_T("===========%s\n"), runAsUserApp);
#endif
				arg_bit = arg_bit | ASUSER;
							
				break;
			}
		}
	}

		if ( (arg_bit&INSTALL) == INSTALL )
		{
			CWinService::InstallService(_T("VerifyServer"), _T("VerifyServer"), _T("VerifyServer"), TRUE);
			::CloseHandle(hMute);
			log(FILE_LOG, _T("process end"));
			return 0;
		}

		if ( (arg_bit & UNSTALL) == UNSTALL )
		{
			CWinService::RemoveService(_T("VerifyServer"), _T("VerifyServer"), TRUE);
			::CloseHandle(hMute);
			log(FILE_LOG, _T("process end"));
			return 0;
		}

		CMyMessagePool message_pool(0);
		CAppEntity appEnter();

		if ( (arg_bit & SERVICE) == SERVICE )
		{
			log(FILE_LOG, _T("argv --service"));
			CWinService::WinRunAsService(_T("VerifyServer"), _T("VerifyServer"), _T("VerifyServer"));
			void *returnval = 0;
			CWinService::WaitServiceMain(&returnval);
		}
		else if ( (arg_bit&ASUSER) == ASUSER )
		{
			void *returnval = 0;
			mainApp.WinAppMain(0, 0);
			CWinService::WaitServiceMain(&returnval);
		}

		if(argfound==false){
#ifndef _DEBUG
			_tprintf(_T("===========%s\n"), _T("argfound"));
#endif		

			CWinService::WinRunAsService(_T("VerifyServer"), _T("VerifyServer"), _T("VerifyServer"));
		}

	::CloseHandle(hMute);
	log(FILE_LOG, _T("process end"));
	return 0;
}

