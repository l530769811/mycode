#ifndef __ESOP_THREADSOCKETRECEVIER_H__
#define __ESOP_THREADSOCKETRECEVIER_H__

#include "SocketRecevier.h"
#include <list>

class CSocketRecevier;
class CIOCompletePort;
class CIOCompletePortOverlapped;
class CThreadSocketRecevier : public CSocketRecevier
{
public:
	CThreadSocketRecevier(CSocketRecevier *pRecevier);
	virtual ~CThreadSocketRecevier();

public:
	virtual void Recevie(DWORD uSokcetID, BYTE *rev_buf, UINT rev_len);
	virtual void connect_coming(DWORD socketid, unsigned int nport);
	virtual void unconnect_coming(DWORD socketid, unsigned int nport);

private:
	CIOCompletePortOverlapped* _getOverlapped();
	void _RemoveOverlapped(CIOCompletePortOverlapped *p);
	void _PushOverlapped(CIOCompletePortOverlapped *p);
	void _ClearOverlapped();

private:
	CSocketRecevier *m_pRecevier;
	static const int nPortCount = 2;
	CIOCompletePort *m_pIoCompletePorts[nPortCount];
	int m_nCurPort;
	CRITICAL_SECTION m_csLock;

	std::list<CIOCompletePortOverlapped*> m_listOverLapped;
	CRITICAL_SECTION m_csLockOverLapped;
};


#endif //__ESOP_THREADSOCKETRECEVIER_H__