#include "stdafx.h"
#include "AppEntity.h"
#include "ThreadSocketRecevier.h"
#include "AppOperate.h"
#include <Windows.h>
#include <TcpServer.h>
#include <DBSqlManager.h>
#include "DBSqlite3Proxy.h"
#include "AppDataFileCopy.h"
#include "veriry_common_define.h"
#include "ClientManager.h"
#include "MyServiceAppMain.h"

CAppEntity::CAppEntity(CMyServiceAppMain *pmain)
	: m_pmain(pmain)
{
	m_pDbManager = new CDBSqlManager();
	m_pclient_manager = new CClientManager(m_pDbManager);

	m_pRecevier = new CThreadSocketRecevier(m_pclient_manager);
	m_tcpServer.Start(m_pRecevier, ("0,0,0,0"), TCP_PORT);

	m_pAppFileData = new CAppDataFileCopy(PROJECT_NAME, DATABASE_NAME);
	mystring strname = m_pAppFileData->GetAppDataFileName();
	m_pdbproxy = new CDBSqlite3Proxy(strname.c_str());
	m_pDbManager->AttachDB(m_pdbproxy);

	
}


CAppEntity::~CAppEntity(void)
{
	NO_NULL(m_pRecevier){
		delete m_pRecevier;
		m_pRecevier = NULL;
	}
	NO_NULL(m_pDbManager){
		m_pDbManager->DetachDB();
	}
	SAFE_DELETE(m_pdbproxy);
	SAFE_DELETE(m_pclient_manager);
}

int CAppEntity::RecevieData(DWORD id, MyString &data)
{
	int ret = -1;

	return ret;
}


int CAppEntity::Operate(const CAppOperate *p)
{
	int ret = -1;

	return ret;
}