#include <DBSqlManager.h>
#include "DBSqlExecOperate.h"


CDBSqlExecOperate::CDBSqlExecOperate(CDBSqlManager *pdbmgr,
	const TCHAR *sql,
	int (*callback)(void *data, int argc, char **argv, char **azColName), 
	void *data)
	: m_pdbmgr(pdbmgr)
	, m_sql(sql)
{
}

CDBSqlExecOperate::CDBSqlExecOperate(void)
	: m_pdbmgr(0)
{
	
}


CDBSqlExecOperate::~CDBSqlExecOperate(void)
{
}


bool CDBSqlExecOperate::Copy(const COperater &p){
	bool bret = false;
	
	return bret;
}

bool CDBSqlExecOperate::_DoOperate(){
	bool bret = false;
	
	if(m_pdbmgr!=0){
		m_pdbmgr->ExecSqlInStack(m_sql.c_str());
	}

	return bret;
}