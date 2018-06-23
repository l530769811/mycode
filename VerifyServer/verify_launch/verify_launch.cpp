// verify_launch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "my_assist_define.h"
#include <vector>
#include <string>
#include <process.h>
#include "NotifyTextCommunicateData.h"
#include "LocalCommunication.h"
#include "MyMessagePool.h"
#include <exception>

const TCHAR		installService[] = 	_T("--install");
const TCHAR		unstallService[] =	_T("--unstall");
const TCHAR		runAsService[]	=  _T("--service");
const TCHAR		runAsUserApp[]  = _T("--run");

#define  NOTIFY_MESSAGE		WM_USER + 0x0001

HANDLE hcommunication_thread;
bool bthreadRunning = false;
HWND hwnd;
std::vector<CCommunicateData*> data_vector;
const int notify_data_count = 20;
CNotifyTextCommunicateData notify_datas[notify_data_count];
CMyMessagePool app;

unsigned int __stdcall _CommunicationThreadProc(void * pParam)
{
	int ret = -1;
	/*int i=100;
	while (i>0)
	{
		if(app.MyPostMessage(NOTIFY_MESSAGE, 0, 0)==FALSE)
		{
			DWORD err = GetLastError();
		}
		i--;
		::Sleep(10);
	}*/

	CLocalCommunication *pcommunicater = (CLocalCommunication*)pParam;
	if(pcommunicater->ConnectedNamedPipe()){
		int notify_index = 0;
		while (bthreadRunning == true)
		{
			CCommunicateData *p = 0;
			for(std::vector<CCommunicateData*>::iterator it_begin=data_vector.begin(); it_begin!=data_vector.end(); it_begin++)
			{
				CCommunicateData *pdata = *it_begin;
				unsigned short nkey = 0;
				if(pcommunicater->ReadCommunicationData(nkey, *pdata)>0)
				{
					p = pdata;
					break;
				}				
			}

			if (p!=0)
			{
				unsigned char data_data[1024] = {0};
				int len = 1023;
				len = p->DecomposeData(data_data, len);
				if (notify_index<notify_data_count-1)
				{
					notify_index++;
				}
				else
				{
					notify_index=0;
				}
				notify_datas[notify_index].ComposeData(data_data, len);
				
				if(app.MyPostMessage(NOTIFY_MESSAGE, 0, 0)==FALSE)
				{
					DWORD err = GetLastError();
				}
			}

			::Sleep(100);
			ret = 1;
		}
	}

	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR str[MAX_PATH] = {0};
	MyString csBasePath;
	::GetModuleFileName(NULL, str, MAX_PATH);
	csBasePath = str;
	int nPos = -1;
	nPos = csBasePath.rfind(_T('\\'));
	csBasePath = csBasePath.substr(0, nPos);
	csBasePath = csBasePath + _T("\\VerifyServer.exe");

	for (int i=1; i< argc; i++)
	{
		TCHAR *ch = argv[i];
		for (int j=0; j<lstrlen(ch); j++)
		{
			ch[j] = tolower(ch[j]);
		}

		csBasePath = csBasePath + _T(" ") + ch; 

	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	//BOOL bprocess = 	::CreateProcess(csBasePath.c_str(),  // No module name (use command line)
	//	0,        // Command line
	//	NULL,           // Process handle not inheritable
	//	NULL,           // Thread handle not inheritable
	//	FALSE,          // Set handle inheritance to FALSE
	//	0,              // No creation flags
	//	NULL,           // Use parent's environment block
	//	NULL,           // Use parent's starting directory 
	//	&si,            // Pointer to STARTUPINFO structure
	//	&pi				// Pointer to PROCESS_INFORMATION structure
	//	);

	BOOL bprocess = 1;
	if (bprocess==TRUE)
	{
		data_vector.push_back(new CNotifyTextCommunicateData);
		CLocalCommunication pcommunicater(_T("verifyserver"));
	
		hwnd = ::GetConsoleWindow();

		if(hcommunication_thread==INVALID_HANDLE_VALUE || hcommunication_thread==0)
		{
			bthreadRunning = true;
			hcommunication_thread = (HANDLE)_beginthreadex(NULL, 0, _CommunicationThreadProc, reinterpret_cast<void *>(&pcommunicater), 0, 0);
			if(hcommunication_thread==INVALID_HANDLE_VALUE || hcommunication_thread==0)
			{
				bthreadRunning = false;
			}
		}
		
		MyMSG msg;
		int count = 0;
		while(app.MyGetMessage(&msg)){
			// convert key ups and downs to chars
			if(msg.message == NOTIFY_MESSAGE)
			{
				if (count<notify_data_count-1)
				{
					count++;
				}
				else
				{
					count=0;
				}

				_tprintf(_T("%s \n"), notify_datas[count].toString().c_str());
			}
			;
		}

		for(std::vector<CCommunicateData*>::iterator it_begin=data_vector.begin(); it_begin!=data_vector.end(); it_begin++)
		{
			CCommunicateData *pdata = *it_begin;
			delete pdata;
		}
	}
	return 0;
}

