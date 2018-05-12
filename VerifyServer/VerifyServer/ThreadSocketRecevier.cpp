#include "stdafx.h"
#include <cassert>
#include "ThreadSocketRecevier.h"
#include "IOCompletePort.h"
#include "StringIOCompletePortOverlapped.h"
#include "thread_lock.h"



#ifdef _DEBUG
#pragma comment(lib, "IOCompletePortU.lib")
#else
#pragma comment(lib, "IOCompletePort.lib")
#endif

CThreadSocketRecevier::CThreadSocketRecevier(CSocketRecevier *pRecevier)
	: m_pRecevier(pRecevier)
	, m_nCurPort(0)
{
	::InitializeCriticalSection(&m_csLock);
	::InitializeCriticalSection(&m_csLockOverLapped);
	for (int i=0; i<nPortCount; i++)
	{
		m_pIoCompletePorts[i] = NULL;
	}

	for (int i=0; i<nPortCount; i++)
	{
		m_pIoCompletePorts[i] = new CIOCompletePort();
		m_pIoCompletePorts[i]->CreateCompletePort();
	}
}

CThreadSocketRecevier::~CThreadSocketRecevier()
{
	for (int i=0; i<nPortCount; i++)
	{
		delete m_pIoCompletePorts[i];
	}
	::DeleteCriticalSection(&m_csLock);
	::DeleteCriticalSection(&m_csLockOverLapped);
}

void CThreadSocketRecevier::Recevie(DWORD uSokcetID, BYTE *rev_buf, UINT rev_len)
{
	assert(m_pRecevier!=NULL);
	if ( m_pRecevier!=NULL )
	{
		TCHAR *pUnicodeBuf = reinterpret_cast<TCHAR*>(rev_buf);
		MyString strData(pUnicodeBuf);
		
		
		CStringIOCompletePortOverlapped p(m_pRecevier);
		p.Update(uSokcetID, rev_buf, rev_len);
		if (m_pIoCompletePorts[m_nCurPort]!=NULL)
		{
			m_pIoCompletePorts[m_nCurPort]->IOCompletePortPost(0, NULL, &p);
		}
		CLock lock(&m_csLock);
		if (m_nCurPort<nPortCount-1)
		{
			m_nCurPort++;
		}
		else
		{
			m_nCurPort=0;
		}
	}
}

void CThreadSocketRecevier::connect_coming(DWORD socketid, unsigned int nport)
{

}
void CThreadSocketRecevier::unconnect_coming(DWORD socketid, unsigned int nport)
{

}


CIOCompletePortOverlapped* CThreadSocketRecevier::_getOverlapped()
{
	CIOCompletePortOverlapped *p = NULL;
	p = m_listOverLapped.front();
	if (p == NULL)
	{
		p = new CStringIOCompletePortOverlapped(m_pRecevier);
	}

	return p;
}

void CThreadSocketRecevier::_RemoveOverlapped(CIOCompletePortOverlapped *p)
{
	if (m_listOverLapped.size()>0)
	{
		m_listOverLapped.remove(p);
	}
}

void CThreadSocketRecevier::_PushOverlapped(CIOCompletePortOverlapped *p)
{
	m_listOverLapped.push_back(p);
}

void CThreadSocketRecevier::_ClearOverlapped()
{
	std::list<CIOCompletePortOverlapped*>::iterator it = m_listOverLapped.begin();
	for ( ;it!=m_listOverLapped.end(); it++)
	{
		CIOCompletePortOverlapped *p = *it;
		delete p;
	}
}

