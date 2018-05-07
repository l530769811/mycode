#ifndef __CLIENTSIFNUPOPERATE_H__
#define __CLIENTSIFNUPOPERATE_H__

#include "Operater.h"
#include <tchar.h>
#include "my_assist_define.h"

class COperater;
class CDBSqlManager;
class CDBSqlExecOperate : public COperater
{
public:
	CDBSqlExecOperate(CDBSqlManager *pdbmgr,
		const TCHAR *sql,
		int (*callback)(void *data, int argc, char **argv, char **azColName), 
		void *data);
	CDBSqlExecOperate(void);
	~CDBSqlExecOperate(void);
	virtual bool Copy(const COperater &p);

private:
	//virtual bool _OperatePrepare(void);
	virtual bool _DoOperate(void);
	//virtual bool _OperateDone(void);

private:
	CDBSqlManager *m_pdbmgr;
	const MyString m_sql;
};

#endif