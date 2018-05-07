#ifndef __COMMON_THREADLOCK_H__
#define __COMMON_THREADLOCK_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN
#include <windows.h>



#ifdef WIN32
typedef HANDLE pthread_t;
typedef HANDLE pthread_mutex_t;
typedef HANDLE pthread_cond_t;
typedef DWORD pthread_key_t;
#endif

class CLock
{
public:
	CLock(const LPCRITICAL_SECTION _m_pCS)
		: m_pCS(_m_pCS)
	{
		::EnterCriticalSection(m_pCS);
	}
	virtual ~CLock()
	{
		::LeaveCriticalSection(m_pCS);
	}

private:
	LPCRITICAL_SECTION m_pCS;
};

// Ïß³ÌËø
class ReadLock
{
public:
	ReadLock(pthread_mutex_t & lock)
		: m_Mutex(lock)
		, m_iLocked(0)
	{
#ifndef WIN32
		m_iLocked = pthread_mutex_t(&m_Mutex);
#else
		m_iLocked = WaitForSingleObject(m_Mutex, INFINITE);
#endif
	}

	~ReadLock()
	{
#ifndef WIN32
		if (0 == m_iLocked)
			pthred_mutex_unlock(&m_Mutex);
#else
		if (WAIT_FAILED != m_iLocked)
			ReleaseMutex(m_Mutex);
#endif
	}

public:
	static void enterCS(pthread_mutex_t& lock)
	{
#ifndef WIN32
		pthread_mutex_lock(&lock);
#else
		WaitForSingleObject(lock, INFINITE);
#endif
	}
	static void leaveCS(pthread_mutex_t& lock)
	{
#ifndef WIN32
		pthread_mutex_unlock(&lock);
#else
		ReleaseMutex(lock);
#endif
	}
	static void createMutex(pthread_mutex_t& lock)
	{
#ifndef WIN32
		pthread_mutex_init(&lock, NULL);
#else
		lock = CreateMutex(NULL, false, NULL);
#endif
	}
	static void releaseMutex(pthread_mutex_t& lock)
	{
#ifndef WIN32
		pthread_mutex_destroy(&lock);
#else
		CloseHandle(lock);
#endif
	}
	static void createCond(pthread_cond_t& cond)
	{
#ifndef WIN32
		pthread_cond_init(&cond, NULL);
#else
		cond = CreateEvent(NULL, false, false, NULL);
#endif
	}
	static void releaseCond(pthread_cond_t& cond)
	{
#ifndef WIN32
		pthread_cond_destroy(&cond);
#else
		CloseHandle(cond);
#endif
	}

private:
	pthread_mutex_t& m_Mutex;            // Alias name of the mutex to be protected
	int m_iLocked;                       // Locking status

	ReadLock & operator=(const ReadLock & other);
};


class CSectLock
{
public:
	CSectLock(){::InitializeCriticalSection(&m_cs); };
	~CSectLock(){::DeleteCriticalSection(&m_cs); };

	void Lock(){::EnterCriticalSection(&m_cs); };
	void Unlock(){::LeaveCriticalSection(&m_cs); };
private:
	CRITICAL_SECTION m_cs;
};


#endif //__COMMON_THREADLOCK_H__