#include "StdAfx.h"
#include "MemAlloctor.h"


CMem::CMem(void)
	: m_bFree(true)
	, m_nSize(nTenKB)
{
	_NewBuf(m_nSize);
}

CMem::CMem(size_t nsize)
	: m_bFree(true)
	, m_nSize(nsize)
{
	if (m_nSize <= 0)
	{
		m_nSize = 1;
	}
	_NewBuf(m_nSize);
}

CMem::~CMem()
{
	
}


bool CMem::IsFree()
{
	return m_bFree;
}

size_t CMem::GetSize() const
{
	return m_nSize;
}

bool CMem::_UseMem(size_t nsize)
{
	bool bret = false;
	if (nsize<=m_nSize)
	{
		m_bFree = false;
		bret = true;
	}

	return bret;
}

bool CMem::_UnUseMem()
{
	bool bret = false;
	if (IsFree()==false
		&& m_byte!=NULL)
	{
		bret = true;
		m_bFree = true;
		::memset(m_byte, 0, m_nSize);
	}
	return bret;
}

void* CMem::_GetBuff()
{
	return m_byte;
}

void CMem::_NewBuf(size_t nsize)
{
	::memset(m_byte, 0, nsize);
}

void CMem::_DeleteBuf()
{
	::memset(m_byte, 0, m_nSize);
}
////////////////////////////////////////////


std::vector<CMem*> CMemPool::m_vecMemBuf;
std::vector<CMem*> CMemPool::m_vecOtherMembuf;

std::list<CMem*> CMemPool::m_listFreeMem;
std::list<CMem*> CMemPool::m_listBusyMem;
CRITICAL_SECTION CMemPool::m_csMemLock;

CMemPool::CMemPool(void)
{
	_initMemBuf();
	::InitializeCriticalSection(&m_csMemLock);	
}


CMemPool::~CMemPool(void)
{
	::DeleteCriticalSection(&m_csMemLock);
	_clearMemBuf();
}

void CMemPool::_initMemBuf()
{
	for (int i=0; i<nInitMaxCount; i++)
	{
		CMem *pMem = new CMem;
		m_vecMemBuf.push_back(pMem);
		m_listFreeMem.push_back(pMem);
	}
}

void CMemPool::_clearMemBuf()
{
	std::vector<CMem*>::iterator it_begin = m_vecMemBuf.begin();
	for (; it_begin!=m_vecMemBuf.end(); )
	{
		CMem *pMem = *it_begin;
		it_begin = m_vecMemBuf.erase(it_begin);
		delete pMem;
	}

	std::vector<CMem*>::iterator it = m_vecOtherMembuf.begin();
	for (; it!=m_vecOtherMembuf.end(); )
	{
		CMem *pMem = *it;
		it = m_vecOtherMembuf.erase(it);
		delete pMem;
	}
}

void* CMemPool::NewMem(size_t nsize)
{
	void *p = NULL;
	try
	{
		::EnterCriticalSection(&m_csMemLock);
		std::list<CMem*>::iterator it_begin = m_listFreeMem.begin();
		for ( ; it_begin!=m_listFreeMem.end(); it_begin++)
		{
			CMem *pMem = *it_begin;
			if (pMem!=NULL)
			{
				if (pMem->GetSize()>=nsize)
				{
					pMem->_UseMem(nsize);
					m_listFreeMem.erase(it_begin);
					p = pMem->_GetBuff();
					m_listBusyMem.push_back(pMem);
					break;
				}
			}
		}

		if (p == NULL)
		{
			CMem *pOtherMem = NULL;
			if (nsize<=nTenKB)
			{
				pOtherMem = new CMem(nTenKB);
				m_vecMemBuf.push_back(pOtherMem);
			}
			else
			{
				pOtherMem = new CMem(nsize);
				m_vecOtherMembuf.push_back(pOtherMem);
			}

			pOtherMem->_UseMem(nsize);			
			m_listBusyMem.push_back(pOtherMem);
			p = pOtherMem->_GetBuff();		
		}
		::LeaveCriticalSection(&m_csMemLock);
	}
	catch(...)
	{
		throw;
	}
	return p;
}

void CMemPool::DeleteMem(void *p)
{
	try
	{
		::EnterCriticalSection(&m_csMemLock);
		
		//this p is same the m_pbyte;
		CMem *pMem = CONTAINING_RECORD(p,
			CMem,
			m_byte);

		if (pMem!=NULL)
		{
			pMem->_UnUseMem();
			m_listBusyMem.remove(pMem);
			m_listFreeMem.push_back(pMem);
		}		
		::LeaveCriticalSection(&m_csMemLock);
	}
	catch(...)
	{
		throw;
	}
}
///////////////////////////////////////////////////////

CMemPool CMemAlloctor::memPool;
CMemAlloctor::CMemAlloctor(void)
{

}

CMemAlloctor::~CMemAlloctor(void)
{

}

void* CMemAlloctor::operator new(size_t nsize)
{
	void *p = NULL;
	try
	{
		p = memPool.NewMem(nsize);	
	}
	catch(...)
	{
		throw;
	}

	return p;
}

void CMemAlloctor::operator delete(void *p)
{
	try
	{
		memPool.DeleteMem(p);
	}
	catch(...)
	{
		throw;
	}
}
