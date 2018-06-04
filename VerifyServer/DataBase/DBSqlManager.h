#ifndef __ESOP_DBSQLMANAGER_H__
#define  __ESOP_DBSQLMANAGER_H__
//#include "SingleModeTemplete.h"
#include "database_define.h"
#include <queue>



///////////////////////////////////////////////////////////////
//sql-data;
///////////////////////////////////////////////////////////////
class CDBProxy;
class CDBDoOperator;
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
	int ExecSqlInStack(CDBDoOperator *pdoop);

	int ExecSql(const TCHAR *sql);
	int ExecSqlInStack(const TCHAR *sql);

	int ExecSqlInStep(const TCHAR *sql, 
		int (*callback)( void *data, int argc, char **argv, char **azColName), 
		void *data);


private:
	static unsigned int __stdcall _SqlexecThreadProc(void * pParam);

private:
	CDBProxy *m_pDB;
	HANDLE m_stackThread;
	bool m_bthreadRunning;

	std::queue<CDBDoOperator*> m_sql_exec_queue;
	CRITICAL_SECTION m_criQueueLock;
	HANDLE m_hEvent;


};


#endif //__ESOP_DBSQLMANAGER_H__