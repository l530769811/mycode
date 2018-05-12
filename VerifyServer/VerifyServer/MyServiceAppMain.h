#ifndef _MYSERVICEAPPMAIN_H__
#define _MYSERVICEAPPMAIN_H__

#include "serviceappmain.h"
class CMyServiceAppMain :
	public ServiceAppMain
{
public:
	CMyServiceAppMain(ProcCallBack call_back);
	virtual ~CMyServiceAppMain(void);
public:
	virtual int WinAppMain(unsigned long argc, TCHAR** argv);
};

#endif //_MYSERVICEAPPMAIN_H__