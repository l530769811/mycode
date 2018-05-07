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

CAppEntity::CAppEntity(void)
{
	m_pRecevier = new CThreadSocketRecevier(this);
	m_tcpServer.Start(m_pRecevier, ("0,0,0,0"), 8020);
	m_pDbManager = new CDBSqlManager();
	

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
	SAFE_DELETE(m_pdbproxy)
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