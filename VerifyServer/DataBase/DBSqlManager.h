#ifndef __ESOP_DBSQLMANAGER_H__
#define  __ESOP_DBSQLMANAGER_H__
//#include "SingleModeTemplete.h"
#include "database_define.h"
#include <queue>
#include <string>
#ifdef _UNICODE
#define DbString std::wstring
#else
#define DbString std::string
#endif

///////////////////////////////////////////////////////////////
//sql-data;
///////////////////////////////////////////////////////////////
class CDBProxy;
class CSopPlayFile;
class _DATABASE_IMPORT_EXPORT_DLL_ CDBSqlManager /*: public CSingleModeTemplete<CDBSqlManager>*/
{
public:
	CDBSqlManager(void);
	~CDBSqlManager(void);
public:
	void AttachDB(CDBProxy *pDB);
	void DetachDB();

	int OpenStack();
	int CloseStack();

	int Lock();
	int Unlock();

	int ExecSql(const TCHAR *sql, 
		int (*callback)( void *data, int argc, char **argv, char **azColName), 
		void *data);
	int ExecSqlInStack(const TCHAR *sql, 
		int (*callback)(void *data, int argc, char **argv, char **azColName), 
		void *data);

	int ExecSql(const TCHAR *sql);
	int ExecSqlInStack(const TCHAR *sql);
private:
	static unsigned int __stdcall _SqlexecThreadProc(void * pParam);

private:
	CDBProxy *m_pDB;

	class CSqlExecNode{
	public:
		friend CDBSqlManager;
		CSqlExecNode(const TCHAR *sql, 
			int (*callback)(void *data, int argc, char **argv, char **azColName), 
			void *data) : m_sql(sql), callback(callback), m_data(data){}
		CSqlExecNode(const TCHAR *sql) : m_sql(sql), callback(0), m_data(0){}
	private:
		DbString m_sql;
		typedef int (*DB_CALLBACK)(void *data, int argc, char **argv, char **azColName);
		DB_CALLBACK callback;
		void *m_data;
	};
	HANDLE m_stackThread;
	bool m_bthreadRunning;

	std::queue<CSqlExecNode*> m_sql_exec_queue;
	CRITICAL_SECTION m_criQueueLock;
	HANDLE m_hEvent;


};


#endif //__ESOP_DBSQLMANAGER_H__