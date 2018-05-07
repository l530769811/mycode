#include "StdAfx.h"
#include "IOCompletePort.h"
#include <process.h>
#include "IOCompletePortOverlapped.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm")

CIOCompletePort::CIOCompletePort(void)
	: m_hCompletePort(INVALID_HANDLE_VALUE)
	, m_nThreadCount(0)
{
	::InitializeCriticalSection(&m_csLockOverlapped);
	CreateCompletePort(20);
}


CIOCompletePort::~CIOCompletePort(void)
{
	DestroyCompletePort();
	::DeleteCriticalSection(&m_csLockOverlapped);
}

bool CIOCompletePort::CreateCompletePort(signed int nthreadcount/* =0 */)
{
	bool bret = true;
	if (m_hCompletePort==INVALID_HANDLE_VALUE
		|| m_hCompletePort==NULL)
	{
		m_hCompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
			NULL, 0, 0);//default thread count is cpu NumberOfProcessors;
		
	}

	if (m_hCompletePort == NULL)//default thread count is cpu NumberOfProcessors;
	{
		bret = false;
	}
	else
	{
		m_nThreadCount = nthreadcount;
		if (m_nThreadCount <= 0)
		{
			// Determine how many processors are on the system.
			SYSTEM_INFO info;
			GetSystemInfo(&info);
			m_nThreadCount = info.dwNumberOfProcessors * 2 + 2;
		}

		for (UINT i = 0; i < m_nThreadCount; i++) 
		{
			HANDLE hThread =  (HANDLE)::_beginthreadex(NULL, 0, WorkerThreadProc, reinterpret_cast<void *>(this), 0, NULL);
			if (hThread) 
			{
				m_hVecThread.push_back(hThread);
			}
			else
			{
				::CloseHandle(hThread);
				bret = false;
			}
		}
	}

	return bret;
}

bool CIOCompletePort::DestroyCompletePort()
{
	bool bret = true;

	UINT remain = m_hVecThread.size();

	UINT index = 0;
	while (remain > 0)
	{
		UINT wait = min(remain, MAXIMUM_WAIT_OBJECTS);
		HANDLE* pHandles = (HANDLE*)_alloca(sizeof(HANDLE) * wait);
		for (UINT i = 0; i < wait; i++)
		{
			pHandles[i] = m_hVecThread[i + index];
		}

		for (UINT i=0; i<wait; i++)
		{
			::PostQueuedCompletionStatus(m_hCompletePort, IOCOMPLETEPORT_EXIT, NULL, NULL);
		}

		WaitForMultipleObjects((DWORD)wait, pHandles, TRUE, INFINITE);

		for (UINT i = 0; i < wait; i++)
			CloseHandle(pHandles[i]);

		remain -= wait;
		index += wait;
	}

	if (m_hCompletePort!=INVALID_HANDLE_VALUE
		&& m_hCompletePort!=NULL)
	{
		::CloseHandle(m_hCompletePort);
		m_hCompletePort = NULL;
	}
	else
	{
		bret = false;
	}

	_ClearOverlappedList();
	return bret;
}

bool CIOCompletePort::BingIoDevice()
{
	bool bret = false;

	return bret;
}

unsigned int __stdcall CIOCompletePort::WorkerThreadProc(LPVOID lpParam)
{
	CIOCompletePort *pThis = reinterpret_cast<CIOCompletePort*>(lpParam);
	if (pThis!=NULL)
	{
		while(TRUE)
		{
			DWORD dwbites = 0;
			WSAOVERLAPPED* pOverlapped = NULL;
			DWORD dwCompletionKey = 0;
			BOOL bResult = ::GetQueuedCompletionStatus(
				pThis->m_hCompletePort,
				&dwbites,
				&dwCompletionKey,
				&pOverlapped,
				INFINITE);

			if (bResult == 0)
			{
				wprintf(_T("GetQueuedCompletionStatus fail\n"));
				break;
			}
			if ( pOverlapped == NULL)
			{
				wprintf(_T("GetQueuedCompletionStatus pOverlapped\n"));
				break;
			}

			CIOCompletePortOverlapped *pol = CONTAINING_RECORD(
				pOverlapped,
				CIOCompletePortOverlapped,
				ol
				);

			if(pol!=NULL)
			{
				pol->OverLappedOparete();
				if (pol->IsExit())
				{
					//pThis->_PushOverlappedList(pol);
					delete pol;
					break;
				}
				delete pol;
				//pThis->_PushOverlappedList(pol);
			}

			if (dwbites == IOCOMPLETEPORT_EXIT)
			{
				break;
			}
		}
	}

	return 0;
}

bool CIOCompletePort::IOCompletePortPost(
	__in DWORD dwNumberOfBytesTransferred, 
	__in ULONG_PTR dwCompletionKey, 
	CIOCompletePortOverlapped *pol )
{
	bool bret = false;
	//CIOCompletePortOverlapped *p = _GetOverlapped();
	//if (p==NULL)
	//{
	//	if (pol!=NULL)
	//	{
	//		p = pol->Clone();
	//	}
	//}
	//else
	//{
	//	p->Copy(*pol);
	//}

	if (m_hCompletePort!=NULL
		&& m_hCompletePort!=INVALID_HANDLE_VALUE)
	{
		if (::PostQueuedCompletionStatus(m_hCompletePort,
			dwNumberOfBytesTransferred,
			dwCompletionKey, 
			&pol->ol))
		{
			bret = true;
		}
	}
	
	return bret;
}

CIOCompletePortOverlapped* CIOCompletePort::_GetOverlapped()
{
	CIOCompletePortOverlapped* p = NULL;
	::EnterCriticalSection(&m_csLockOverlapped);
	if (m_listOverlapped.size()>0)
	{
		p = m_listOverlapped.front();
		m_listOverlapped.pop_front();
	}
	::LeaveCriticalSection(&m_csLockOverlapped);
	return p;
}

void CIOCompletePort::_PushOverlappedList(CIOCompletePortOverlapped *p)
{
	::EnterCriticalSection(&m_csLockOverlapped);
	m_listOverlapped.push_back(p);
	::LeaveCriticalSection(&m_csLockOverlapped);
}

void CIOCompletePort::_PopOverlappedList()
{
	::EnterCriticalSection(&m_csLockOverlapped);
	if (m_listOverlapped.size()>0)
		m_listOverlapped.pop_back();
	::LeaveCriticalSection(&m_csLockOverlapped);
}

void CIOCompletePort::_RemoveOverlapped(CIOCompletePortOverlapped *p)
{
	::EnterCriticalSection(&m_csLockOverlapped);
	if (m_listOverlapped.size()>0)
		m_listOverlapped.remove(p);
	::LeaveCriticalSection(&m_csLockOverlapped);
}

void CIOCompletePort::_ClearOverlappedList()
{
	std::list<CIOCompletePortOverlapped*>::iterator it = m_listOverlapped.begin();
	while (it!=m_listOverlapped.end())
	{
		CIOCompletePortOverlapped *p = *it;
		delete p;
		it++;
	}
}
