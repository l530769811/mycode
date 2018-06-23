#ifndef _COMMON_MYMESSAGEPOOL_H__
#define _COMMON_MYMESSAGEPOOL_H__
#include "common_define.h"
#include "UseCount.h"

typedef long (*ProcCallBack)(UINT msg, WPARAM wp, LPARAM lp);

#define WM_MYQUIT   0x111111

typedef struct MyMSG {
	MyMSG(void){}
	MyMSG(MyMSG *pmsg)
		: nextMsg(new MyMSG){}
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
	CUseCount<MyMSG> nextMsg; 
} MyMSG, *PMyMSG, NEAR *NPMyMSG, FAR *LPMyMSG;

class _COMMON_IMPORT_EXPORT_DLL_ CMyMessagePool
{
public:
	CMyMessagePool(ProcCallBack call_back = 0 );
	virtual ~CMyMessagePool(void);


	long MySendMessage(UINT msg, WPARAM wp, LPARAM lp);
	bool MyPostMessage(UINT msg, WPARAM wp, LPARAM lp);
	long MyDispatchMessage(const MyMSG *pmsg);
	bool MyGetMessage(MyMSG *pmsg);

private:
	ProcCallBack m_call_back;
	CUseCount<MyMSG> headMsg;

	CRITICAL_SECTION m_critical_section;
	HANDLE m_hWaitHandle;
};

#endif 
//_COMMON_MYMESSAGEPOOL_H__