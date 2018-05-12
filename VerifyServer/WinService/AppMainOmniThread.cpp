#include "AppMainOmniThread.h"


CAppMainOmniThread::CAppMainOmniThread(void (*fn)(void*), void* arg)
	:my_fn_void(fn)
	, parg(arg)
{
}


CAppMainOmniThread::~CAppMainOmniThread(void)
{
}

void CAppMainOmniThread::run(void* arg)
{

}

void* CAppMainOmniThread::run_undetached(void* arg)
{

}
