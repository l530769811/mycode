#ifndef __BUFFER_H__
#define __BUFFER_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

class CBuffer
{
public:
	CBuffer();
	virtual ~CBuffer();

	UINT GetBufferLen();
	void ClearBuffer();
	UINT Delete(UINT nSize);
	UINT Read(BYTE* pData, UINT nSize);
	BOOL Write(BYTE* pData, UINT nSize);
	BOOL Insert(BYTE* pData, UINT nSize);
	int Scan(BYTE* pScan, UINT nPos);
	void Copy(CBuffer& buffer);
	BYTE* GetBuffer(UINT nPos = 0);
	void FileWrite(const char* pFilePath);

protected:
	UINT ReAllocateBuffer(UINT nRequestSize);
	UINT DeAllocateBuffer(UINT nRequestSize);
	UINT GetMemorySize();

protected:
	BYTE* m_pBase;
	BYTE* m_pData;
	UINT   m_nSize;
};

#endif	// __BUFFER_H__