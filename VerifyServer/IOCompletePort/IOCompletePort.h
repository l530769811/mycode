#ifndef _COMMON_CIOCOMPLETEPORT_H__
#define _COMMON_CIOCOMPLETEPORT_H__





#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <vector>
#include <list>
#include "define.h"

#define IOCOMPLETEPORT_EXIT		0x0FFFFFFF

class CIOCompletePortOverlapped;
class _IO_IMPORT_EXPORT_DLL_ CIOCompletePort
{
public:
	CIOCompletePort(void);
	~CIOCompletePort(void);

public:
	bool CreateCompletePort(signed int nthreadcount=0);
	bool DestroyCompletePort();
	bool BingIoDevice();

	bool IOCompletePortPost(
		__in     DWORD dwNumberOfBytesTransferred,
		__in     ULONG_PTR dwCompletionKey,
		CIOCompletePortOverlapped *pol
		);
	
private:
	static unsigned int __stdcall WorkerThreadProc(LPVOID lpParam);

	CIOCompletePortOverlapped* _GetOverlapped();
	void _PushOverlappedList(CIOCompletePortOverlapped *p);
	void _PopOverlappedList();
	void _RemoveOverlapped(CIOCompletePortOverlapped *p);
	void _ClearOverlappedList();
private:
	HANDLE m_hCompletePort;
	signed int m_nThreadCount;

	std::vector<HANDLE> m_hVecThread;

	std::list<CIOCompletePortOverlapped*> m_listOverlapped;
	CRITICAL_SECTION m_csLockOverlapped;
};

#endif //_COMMON_CIOCOMPLETEPORT_H__