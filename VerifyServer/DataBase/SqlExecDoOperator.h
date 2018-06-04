#ifndef _SQLEXECDOOPERATOR_H__
#define _SQLEXECDOOPERATOR_H__
#include "database_define.h"
#include "DBDoOperator.h"

class CDBSqlManager;
class  CSqlExecDoOperator : public CDBDoOperator
{
public:
	CSqlExecDoOperator(const TCHAR *sql, 
		int (*callback)(void *data, int argc, char **argv, char **azColName), 
		void *data);
	virtual ~CSqlExecDoOperator(void);

public:
	virtual int Exec(CDBSqlManager *pdb);

private:
	DbString m_sql;
	typedef int (*DB_CALLBACK)(void *data, int argc, char **argv, char **azColName);
	DB_CALLBACK callback;
	void *m_data;
};

#endif //_SQLEXECDOOPERATOR_H__