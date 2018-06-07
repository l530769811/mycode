#ifndef _CONSUMECARDDBDOOPERATOR_H__
#define _CONSUMECARDDBDOOPERATOR_H__

#include "dbdooperator.h"
#include "my_assist_define.h"
class CConsumeCardDBDoOperator :
	public CDBDoOperator
{
public:
	CConsumeCardDBDoOperator(MyString strSql);
	virtual ~CConsumeCardDBDoOperator(void);

	static int ConsumeCardCallback(void *data, int argc, char **argv, char **azColName);
	int GetResult() const;

public:
	virtual int Exec(CDBSqlManager *pdb);

private:
	MyString m_sql;
	int m_result;
};

#endif _CONSUMECARDDBDOOPERATOR_H__