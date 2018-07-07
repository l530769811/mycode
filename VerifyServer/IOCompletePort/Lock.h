#ifndef _CRITICALSECTION_LOCK_H__
#define _CRITICALSECTION_LOCK_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include <Windows.h>

class CCriticalSectionLock
{
public:
	CCriticalSectionLock(void);
	~CCriticalSectionLock(void);

public:
	void Lock();
	void Unlock();

private:
	CRITICAL_SECTION m_csLock;
};

#endif _CRITICALSECTION_LOCK_H__