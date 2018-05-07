#ifndef COMMON_USE_COUNT_H_
#define COMMON_USE_COUNT_H_

#include "my_platform.h"
#include "common_define.h"

/////////////////////////////////////
/*
《C++沉思录》上的分离引用计数类,添加了多线程安全;
time:2014-12.01;
*/
/////////////////////////////////////
template <typename T>
class _COMMON_IMPORT_EXPORT_DLL_ CUseCount
{
public:
	CUseCount(T* pp);
	CUseCount(const CUseCount &u);
	~CUseCount(void);

	T*  operator->(); 

protected:
	bool Only();
	bool Reattach(const CUseCount& u);
	bool Makeonly();

private:
	CUseCount& operator=(const CUseCount &u);

private:
	atomic_t *m_pCount;
	T *p;
	
};

#endif