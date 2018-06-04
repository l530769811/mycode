#include "StdAfx.h"
#include "SqlExecDoOperator.h"
#include "DBSqlManager.h"

CSqlExecDoOperator::CSqlExecDoOperator(const TCHAR *sql, 
	int (*callback)(void *data, int argc, char **argv, char **azColName), 
	void *data)
	 : m_sql(sql)
	 , callback(callback)
	 , m_data(data)
{
}


CSqlExecDoOperator::~CSqlExecDoOperator(void)
{
}

int CSqlExecDoOperator::Exec(CDBSqlManager *pdb)
{
	int i = 0;
	if(pdb!=0)
	{
		i = pdb->ExecSql(m_sql.c_str(), callback, m_data); 
	}
	return i;
}
