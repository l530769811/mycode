#ifndef _APPMAINOMNITHREAD_H__
#define _APPMAINOMNITHREAD_H__

#include "omnithread.h"
class CAppMainOmniThread :
	public omni_thread
{
public:
	CAppMainOmniThread(void (*fn)(void*), void* arg);
	~CAppMainOmniThread(void);

private:

	virtual void run(void* arg);
	virtual void* run_undetached(void* arg);

	void (*my_fn_void)(void*);

	void *parg;
};

#endif //_APPMAINOMNITHREAD_H__