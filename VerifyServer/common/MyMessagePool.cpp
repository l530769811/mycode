#include "MyMessagePool.h"
#include <tchar.h>

CMyMessagePool::CMyMessagePool(ProcCallBack call_back /* = 0*/)
	: m_call_back(call_back)
	, headMsg(new MyMSG)
	, m_hWaitHandle(INVALID_HANDLE_VALUE)
{
	m_hWaitHandle = ::CreateEvent(0, false, false, 0);
	::InitializeCriticalSection(&m_critical_section); 
}


CMyMessagePool::~CMyMessagePool(void)
{
	::DeleteCriticalSection(&m_critical_section);
}


long CMyMessagePool::MySendMessage(UINT umsg, WPARAM wp, LPARAM lp)
{
	long bret = -1;
	MyMSG msg;
	msg.lParam = lp;
	msg.message = umsg;
	msg.wParam = wp;
	MyDispatchMessage(&msg);
	return bret;
}

bool CMyMessagePool::MyPostMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	bool bret = false;
	if(m_hWaitHandle!=NULL && m_hWaitHandle!=INVALID_HANDLE_VALUE)
	{
		//head is a CUseCount has a CUseCount what has a null pointer in it 
		::EnterCriticalSection(&m_critical_section);
		
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
		::SetEvent(m_hWaitHandle);
		::LeaveCriticalSection(&m_critical_section);
	}

	return bret;
}

long CMyMessagePool::MyDispatchMessage(const MyMSG *pmsg)
{
	long bret = false;

	if (m_call_back!=0 && pmsg!=0)
	{
		bret = m_call_back(pmsg->message, pmsg->wParam, pmsg->lParam);
	}
	return bret;
}

bool CMyMessagePool::MyGetMessage(MyMSG *pmsg)
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
	will be has same msg in  out_msg->nextMsg end headMsg->nextMsg->nextMsg
	*/	
	headMsg->nextMsg = headMsg->nextMsg->nextMsg; 
	out_msg->nextMsg = CUseCount<MyMSG>();
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


