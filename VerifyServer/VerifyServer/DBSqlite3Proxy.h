#ifndef __DBSQLITE3PROXY_H__
#define __DBSQLITE3PROXY_H__
#include "DBProxy.h"

class CDBSqlite3Proxy : public CDBProxy
{
public:
	CDBSqlite3Proxy(const TCHAR *name);
	virtual ~CDBSqlite3Proxy(void);

	virtual int ExecSql(const TCHAR *sql, 
		int (*callback)(void *data, int argc, char **argv, char **azColName), 
		void *data);
	virtual int ExecSql(const TCHAR *sql);

	virtual int ExecSqlInStep(const TCHAR *sql, int (*callback)(void *data, int argc, char **argv, char **azColName), void *data);

private:
	sqlite3 *m_pdb;
};

#endif //__DBSQLITE3PROXY_H__