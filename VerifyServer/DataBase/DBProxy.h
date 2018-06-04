#ifndef __DBLIB_CDBPROXY_H__
#define __DBLIB_CDBPROXY_H__

#include "database_define.h"
//char* UnicodeToUtf8(const TCHAR *unicode, char *uft8, int len);
//TCHAR* Utf8ToUnicode( const char *uft8, TCHAR *unicode, int len);

typedef struct sqlite3 sqlite3;
class _DATABASE_IMPORT_EXPORT_DLL_ CDBProxy
{
public:
	CDBProxy(void);
	virtual ~CDBProxy(void);
	virtual int ExecSql(const TCHAR *sql, 
		int (*callback)(void *data, int argc, char **argv, char **azColName), 
		void *data) = 0;
	virtual int ExecSql(const TCHAR *sql) = 0;

	virtual int ExecSqlInStep(const TCHAR *sql, 
		int (*callback)(void *data, int argc, char **argv, char **azColName), 
		void *data);
	virtual int ExecSqlInStep(const TCHAR *sql);
};

#endif //__DBLIB_CDBPROXY_H__