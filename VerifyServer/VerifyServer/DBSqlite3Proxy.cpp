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
		fprintf(stderr, "SQLite error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	return ret;
}
int CDBSqlite3Proxy::ExeSql(const TCHAR *sql)
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
