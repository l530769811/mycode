#include <tchar.h>

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

ServiceAppMain::ServiceAppMain(ServiceAppMain *pmain, ProcCallBack call_back)
	: m_call_back(call_back)
	, headMsg(new MyMSG)
	, m_hWaitHandle(INVALID_HANDLE_VALUE)
{
	m_hWaitHandle = ::CreateEvent(0, false, false, _T("waithandle"));
	::InitializeCriticalSection(&m_critical_section); 
	ServiceAppMain::m_pmain = pmain;
}


ServiceAppMain::~ServiceAppMain(void)
{
	::DeleteCriticalSection(&m_critical_section);
}

long ServiceAppMain::MySendMessage(UINT umsg, WPARAM wp, LPARAM lp)
{
	long bret = -1;
	MyMSG msg;
	msg.lParam = lp;
	msg.message = umsg;
	msg.wParam = wp;
	MyDispatchMessage(&msg);
	return bret;
}

bool ServiceAppMain::MyPostMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	bool bret = false;
	if(m_hWaitHandle!=NULL && m_hWaitHandle!=INVALID_HANDLE_VALUE)
	{
		//head is a CUseCount has a CUseCount what has a null pointer in it 
		::EnterCriticalSection(&m_critical_section);
		::SetEvent(m_hWaitHandle);
		CUseCount<MyMSG> nextMsg = headMsg;
		
		while(nextMsg->nextMsg.isNull()==false)
		{
			nextMsg = nextMsg->nextMsg;
		}
		CUseCount<MyMSG> newMsg(new MyMSG);
		newMsg->lParam = lp;
		newMsg->message = msg;
		newMsg->wParam = wp;
		nextMsg->nextMsg = newMsg;
		::LeaveCriticalSection(&m_critical_section);
	}

	return bret;
}

long ServiceAppMain::MyDispatchMessage(const MyMSG *pmsg)
{
	long bret = false;

	if (m_call_back!=0 && pmsg!=0)
	{
		bret = m_call_back(pmsg->message, pmsg->wParam, pmsg->lParam);
	}
	return bret;
}

bool ServiceAppMain::MyGetMessage(MyMSG *pmsg)
{
	bool bret = true;

	if(headMsg->nextMsg.isNull() == true)
	{		

		if(m_hWaitHandle!= NULL && m_hWaitHandle!=INVALID_HANDLE_VALUE)
		{
			::WaitForSingleObject(m_hWaitHandle, INFINITE);
		}
		else
		{
			::Sleep(1000);
		}
	}
	
	//head is a CUseCount has a CUseCount what has a null pointer in it 
	::EnterCriticalSection(&m_critical_section);
	CUseCount<MyMSG> out_msg = headMsg->nextMsg;
	/*
	must be set out_msg->nextMsg  to empty msg
	because headMsg->nextMsg = headMsg->nextMsg->nextMsg 
	will be has same msg to out_msg
	*/
	out_msg->nextMsg = CUseCount<MyMSG>();
	headMsg->nextMsg = headMsg->nextMsg->nextMsg; 
	if(m_hWaitHandle!= NULL && m_hWaitHandle!=INVALID_HANDLE_VALUE)
	{
		::ResetEvent(m_hWaitHandle);
	}
	::LeaveCriticalSection(&m_critical_section);
	if(out_msg.isNull()==false && pmsg!=0)
	{
		pmsg->lParam = out_msg->lParam;
		pmsg->message = out_msg->message;
		pmsg->wParam = out_msg->wParam;
		if(pmsg->message != WM_MYQUIT)
		{
			bret = true;
		}
		else
		{
			bret = false;
		}
	}

	return bret;
}
