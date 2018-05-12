#ifndef __VERIFY_APPENTITY_H__
#define __VERIFY_APPENTITY_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <TcpServer.h>

class CMyServiceAppMain;
class CAppDataFileCopy;
class CDBProxy;
class CDBSqlManager;
class CSocketRecevier;
class CTcpServer;
class CAppOperate;
class CClientManager;
class CAppEntity
{
public:
	CAppEntity(CMyServiceAppMain *pmain);
	~CAppEntity(void);

	int RecevieData(DWORD id, MyString &data);
	int Operate(const CAppOperate *p);

private:

	CMyServiceAppMain *m_pmain;

	CSocketRecevier *m_pRecevier;
	CTcpServer m_tcpServer;
	CDBSqlManager *m_pDbManager;
	CDBProxy *m_pdbproxy;
	CAppDataFileCopy *m_pAppFileData;
	CClientManager *m_pclient_manager;


};

#endif