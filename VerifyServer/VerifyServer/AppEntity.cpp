#include "stdafx.h"
#include <iterator>
#include "AppEntity.h"
#include "ThreadSocketRecevier.h"
#include "AppOperate.h"
#include <Windows.h>
#include <TcpServer.h>
#include <DBSqlManager.h>
#include "DBSqlite3Proxy.h"
#include "AppDataFileCopy.h"
#include "veriry_common_define.h"
#include "VIPCardManager.h"
#include "ClientManager.h"
#include "MyServiceAppMain.h"
#include "sqlite_sql.h"
#include "ProcessLogCollecter.h"


CAppEntity::CAppEntity()
{
	m_pDbManager = new CDBSqlManager();
	m_pAppFileData = new CAppDataFileCopy(PROJECT_NAME, DATABASE_NAME);
	
	mystring strname  = m_pAppFileData->GetAppDataFileName();

	m_pdbproxy = new CDBSqlite3Proxy(strname.c_str());
	m_pDbManager->AttachDB(m_pdbproxy);	

	m_pvipcard_manager = new CVIPCardManager(m_pDbManager);
	m_pclient_manager = new CClientManager(m_pDbManager, m_pvipcard_manager);

	m_pRecevier = new CThreadSocketRecevier(m_pclient_manager);
	m_tcpServer = new CTcpServer();
	if(m_tcpServer->Start(m_pRecevier, ("0.0.0.0"), TCP_PORT)==FALSE)
	{
		log(FILE_LOG, _T(" tcpserver fail"));
	}

	m_pclient_manager->BindSocket(m_tcpServer);

	m_pDbManager->ExecSql(create_user_table);
	m_pDbManager->ExecSql(create_user_index);

	m_pDbManager->ExecSql(create_cardregister_table);
	m_pDbManager->ExecSql(create_cardregister_index);	
}


CAppEntity::~CAppEntity(void)
{
	NO_NULL(m_tcpServer){
		m_tcpServer->Stop();
		delete m_tcpServer;
		m_tcpServer = 0;
	}

	NO_NULL(m_pRecevier){
		delete m_pRecevier;
		m_pRecevier = NULL;
	}
	NO_NULL(m_pDbManager){
		m_pDbManager->DetachDB();
	}
	SAFE_DELETE(m_pdbproxy);
	SAFE_DELETE(m_pvipcard_manager);
	SAFE_DELETE(m_pclient_manager);

	
}

int CAppEntity::RecevieData(unsigned long id, MyString &data)
{
	int ret = -1;

	return ret;
}


int CAppEntity::Operate(const CAppOperate *p)
{
	int ret = -1;

	return ret;
}