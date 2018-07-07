#include "StdAfx.h"
#include "Lock.h"


CCriticalSectionLock::CCriticalSectionLock(void)
{
	::InitializeCriticalSection(&m_csLock);
}


CCriticalSectionLock::~CCriticalSectionLock(void)
{
	::DeleteCriticalSection(&m_csLock);
}

void CCriticalSectionLock::Lock()
{
	::EnterCriticalSection(&m_csLock);
}

void CCriticalSectionLock::Unlock()
{
	::LeaveCriticalSection(&m_csLock);
}