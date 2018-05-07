#include "ClientVerifyReponseOperater.h"
#include "ClientManager.h"
#include "LoginVerifyReponseNetSocketData.h"
#include "UdpNetSocketProxy.h"

CClientVerifyReponseOperater::CClientVerifyReponseOperater(CClientManager *pclient, CUDPSocket *psocket,  CClientVerifyReponseOperater **p)
	: m_pclient(pclient)
	, m_pUdpVerify(psocket)
	, pthis(p)
{
}


CClientVerifyReponseOperater::~CClientVerifyReponseOperater(void)
{
}

bool CClientVerifyReponseOperater::Copy(const COperater &p)
{
	bool bret = false;

	return bret;
}

bool CClientVerifyReponseOperater::_DoOperate()
{
	bool bret = false;

	return bret;
}

int CClientVerifyReponseOperater::ClientVerifyCallback(void *data, int argc, char **argv, char **azColName)
{
	int ret = 0;

	if(data!=0 && typeid(data)==typeid(CClientVerifyReponseOperater))
	{
		CClientVerifyReponseOperater *pdata = reinterpret_cast <CClientVerifyReponseOperater*>(data);
		CLoginVerifyReponseNetSocketData data(0);
		CUdpNetSocketProxy proxy(pdata->m_pUdpVerify, L"", 000);
		pdata->m_pclient->ClientReponse(data, proxy);		
	}

	return ret;
}