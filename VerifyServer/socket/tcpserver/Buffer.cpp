#include "stdafx.h"
#include "Buffer.h"
#include <math.h>


CBuffer::CBuffer()
{
	m_nSize = 0;
	m_pData = m_pBase = NULL;
}

CBuffer::~CBuffer()
{
	if (m_pBase)
		delete m_pBase;
}

BOOL CBuffer::Write(BYTE* pData, UINT nSize)
{
	ReAllocateBuffer(nSize + GetBufferLen());
	memcpy(m_pData, pData, nSize);
	m_pData += nSize;
	return nSize;
}

BOOL CBuffer::Insert(BYTE* pData, UINT nSize)
{
	ReAllocateBuffer(nSize + GetBufferLen());

	memmove(m_pBase+nSize, m_pBase, GetMemorySize()-nSize);
	memcpy(m_pBase, pData, nSize);

	m_pData += nSize;

	return nSize;
}

UINT CBuffer::Read(BYTE* pData, UINT nSize)
{
	if (nSize > GetMemorySize())
		return 0;

	if (nSize > GetBufferLen())
		nSize = GetBufferLen();

	if (nSize)
	{
		memcpy(pData, m_pBase, nSize);
		memmove(m_pBase, m_pBase+nSize, GetMemorySize()-nSize);
		m_pData -= nSize;
	}

	DeAllocateBuffer(GetBufferLen());

	return nSize;
}

UINT CBuffer::Delete(UINT nSize)
{
	if (nSize > GetMemorySize())
		return 0;

	if (nSize > GetBufferLen())
		nSize = GetBufferLen();

	if (nSize)
	{
		memmove(m_pBase, m_pBase+nSize, GetMemorySize()-nSize);
		m_pData -= nSize;
	}

	DeAllocateBuffer(GetBufferLen());

	return nSize;
}

int CBuffer::Scan(BYTE* pScan, UINT nPos)
{
	if (nPos > GetBufferLen())
		return -1;

	BYTE* pData = (BYTE*)strstr((char*)(m_pBase+nPos), (char*)pScan);
	int nOffset = 0;
	if (pData)
		nOffset = (pData - m_pBase) + strlen((char*)pScan);

	return nOffset;
}

void CBuffer::Copy(CBuffer& buffer)
{
	int nReSize = buffer.GetMemorySize();
	int nSize = buffer.GetBufferLen();
	ClearBuffer();
	ReAllocateBuffer(nReSize);

	m_pData = m_pBase + nSize;
	memcpy(m_pBase, buffer.GetBuffer(), buffer.GetBufferLen());
}

BYTE* CBuffer::GetBuffer(UINT nPos /* = 0 */)
{
	return m_pBase+nPos;
}

void CBuffer::FileWrite(const char* pFilePath)
{
}

void CBuffer::ClearBuffer()
{
	m_pData = m_pBase;
	DeAllocateBuffer(1024);
}

UINT CBuffer::GetMemorySize()
{
	return m_nSize;
}

UINT CBuffer::GetBufferLen()
{
	if (m_pBase == NULL)
		return 0;

	int nSize = m_pData - m_pBase;

	return nSize;
}

UINT CBuffer::ReAllocateBuffer(UINT nRequestSize)
{
	if (nRequestSize < GetMemorySize())
		return 0;

	// Allocate new size
	UINT nNewSize = (UINT)ceil(nRequestSize / 1024.0) * 1024;

	// New  copy data over
	BYTE* pNewBuffer = new BYTE[nNewSize];

	UINT nBufferLen = GetBufferLen();
	memcpy(pNewBuffer, m_pBase, nBufferLen);

	if (m_pBase)
		delete m_pBase;

	m_pBase = pNewBuffer;
	m_pData = m_pBase + nBufferLen;
	m_nSize = nNewSize;

	return m_nSize;
}

UINT CBuffer::DeAllocateBuffer(UINT nRequestSize)
{
	if (nRequestSize < GetBufferLen())
		return 0;

	// Allocate new size
	UINT nNewSize = (UINT)ceil(nRequestSize / 1024.0) * 1024;
	if (nNewSize < GetMemorySize())
		return 0;

	// New  copy data over
	BYTE* pNewBuffer = new BYTE[nNewSize];

	UINT nBufferLen = GetBufferLen();
	memcpy(pNewBuffer, m_pBase, nBufferLen);

	if (m_pBase)
		delete m_pBase;

	m_pBase = pNewBuffer;
	m_pData = m_pBase + nBufferLen;
	m_nSize = nNewSize;

	return m_nSize;
}