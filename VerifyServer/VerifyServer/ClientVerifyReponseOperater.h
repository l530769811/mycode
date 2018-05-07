#ifndef _CLIENTVERIFTREPONSEOPERATER_H__
#define _CLIENTVERIFTREPONSEOPERATER_H__

#include "operater.h"
#include "MemAlloctor.h"

class CUDPSocket;
class CClientManager;
class CClientVerifyReponseOperater :
	public COperater, public CMemAlloctor
{
public:
	CClientVerifyReponseOperater(CClientManager *pclient, CUDPSocket *spocket, CClientVerifyReponseOperater **p);
	virtual ~CClientVerifyReponseOperater(void);
	virtual bool Copy(const COperater &p);

	static int ClientVerifyCallback(void *data, int argc, char **argv, char **azColName);

private:	
	virtual bool _DoOperate(void);

private:
	CClientManager *m_pclient;
	CUDPSocket * m_pUdpVerify;
	CClientVerifyReponseOperater **pthis;
};

#endif //_CLIENTVERIFTOPERATER_H__