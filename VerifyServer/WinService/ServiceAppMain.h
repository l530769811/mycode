#ifndef __SERVICEAPPMAIN_H__
#define __SERVICEAPPMAIN_H__

#ifndef WIN32_LEAN_AND_MEAN
#define  WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "winservice_define.h"
#include <UseCount.h>
#include <UseCount.cpp>

//extern int WinAppMain(unsigned long argc, TCHAR** argv);
typedef long (*ProcCallBack)(UINT msg, WPARAM wp, LPARAM lp);

#define WM_MYQUIT   0x111111

typedef struct MyMSG {
	MyMSG(void){}
	MyMSG(MyMSG *pmsg)
	: nextMsg(new MyMSG){}
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
	CUseCount<MyMSG> nextMsg; 
} MyMSG, *PMyMSG, NEAR *NPMyMSG, FAR *LPMyMSG;

class _WINSERVICE_IMPORT_EXPORT_DLL_ ServiceAppMain 
{
public:
	ServiceAppMain(ServiceAppMain *pmain, ProcCallBack call_back = 0 );
	~ServiceAppMain(void);

	long MySendMessage(UINT msg, WPARAM wp, LPARAM lp);
	bool MyPostMessage(UINT msg, WPARAM wp, LPARAM lp);
	long MyDispatchMessage(const MyMSG *pmsg);
	bool MyGetMessage(MyMSG *pmsg);

	friend int WinAppMain(unsigned long argc, TCHAR** argv);

	virtual int WinAppMain(unsigned long argc, TCHAR** argv) = 0;

private:
	ProcCallBack m_call_back;
	CUseCount<MyMSG> headMsg;

	CRITICAL_SECTION m_critical_section;
	HANDLE m_hWaitHandle;

	static ServiceAppMain *m_pmain;
};


#endif