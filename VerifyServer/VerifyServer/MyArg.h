#ifndef _MYARG_H__
#define _MYARG_H__

class CMyArg
{
public:
	CMyArg(void);
	virtual ~CMyArg(void);
	
public:
	virtual int GetIntArg();
	virtual int* GetIntArgs(int *args);
	virtual void * GetObjectArgs(void * args);

};

#endif //_MYARG_H__