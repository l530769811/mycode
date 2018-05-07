#ifndef __VERIFY_APPENTITY_H__
#define __VERIFY_APPENTITY_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <TcpServer.h>

class CAppDataFileCopy;
class CDBProxy;
class CDBSqlManager;
class CSocketRecevier;
class CTcpServer;
class CAppOperate;
class CAppEntity
{
public:
	CAppEntity(void);
	~CAppEntity(void);

	int RecevieData(DWORD id, MyString &data);
	int Operate(const CAppOperate *p);

private:

	CSocketRecevier *m_pRecevier;
	CTcpServer m_tcpServer;
	CDBSqlManager *m_pDbManager;
	CDBProxy *m_pdbproxy;
	CAppDataFileCopy *m_pAppFileData;
};

#endif