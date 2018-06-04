#include "DBSqlite3Proxy.h"
#include "GlobalFunc.h"
#include "sqlite3.h"


#pragma comment(lib, "build_sqlite3.lib")

CDBSqlite3Proxy::CDBSqlite3Proxy(const TCHAR *name)
	: m_pdb(NULL)
{
	char str[MAX_PATH] = {0};
	GlobalUnicodeToUtf8(name, str, MAX_PATH);
	int ret = sqlite3_open(str, &m_pdb);
}


CDBSqlite3Proxy::~CDBSqlite3Proxy(void)
{
	sqlite3_close(m_pdb);
}

int CDBSqlite3Proxy::ExecSql(const TCHAR *sql, 
	int (*callback)(void *data, int argc, char **argv, char **azColName), 
	void *data)
{
	int ret = -1;
	
	char *zErrMsg = NULL;
	char strSql[1024] = {0};
	GlobalUnicodeToUtf8(sql, strSql, 1023);
	ret = sqlite3_exec(m_pdb, strSql, callback, data, &zErrMsg);
	if (ret != SQLITE_OK)
	{
#ifdef _DEBUG
		fprintf(stderr, "SQLite error: %s\n", zErrMsg);
#endif		
		sqlite3_free(zErrMsg);
	}

	return ret;
}
int CDBSqlite3Proxy::ExecSql(const TCHAR *sql)
{
	int ret = -1;

	char *zErrMsg = NULL;
	char strSql[1024] = {0};
	GlobalUnicodeToUtf8(sql, strSql, 1023);
	ret = sqlite3_exec(m_pdb, strSql, NULL, NULL, &zErrMsg);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "SQLite error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	return ret;
}

int CDBSqlite3Proxy::ExecSqlInStep(const TCHAR *sql, int (*callback)(void *data, int argc, char **argv, char **azColName), void *data)
{
	int nret = -1;
	//char *zErrMsg = NULL;
	//char strSql[1024] = {0};
	//GlobalUnicodeToUtf8(sql, strSql, 1023);
	//sqlite3_stmt * stmt = 0;
	//if (m_pdb!=0)
	//{
	//	sqlite3_prepare(m_pdb, strSql,strlen(strSql)* sizeof(char), &stmt, 0);
	//	sqlite3_step(stmt);
	//	
	//	int column_count = sqlite3_column_count(stmt);
	//	for(int i=0; i< column_count; i++)
	//	{
	//		const char * name = sqlite3_column_name(stmt, i);
	//		const unsigned char *value = sqlite3_column_text(stmt, i);	
	//	}
	//		
	//}

	//sqlite3_finalize(stmt);

	return nret;
}
