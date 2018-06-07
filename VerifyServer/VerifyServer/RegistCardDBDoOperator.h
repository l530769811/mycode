#ifndef _REGISTCARDDBDOOPERATOR_H__
#define _REGISTCARDDBDOOPERATOR_H__

#include "dbdooperator.h"
#include "my_assist_define.h"

class CRegistCardDBDoOperator :
	public CDBDoOperator
{
public:
	CRegistCardDBDoOperator(MyString strSql);
	virtual ~CRegistCardDBDoOperator(void);
	static int RegistCardCallback(void *data, int argc, char **argv, char **azColName);
	int GetResult() const;
public:
	virtual int Exec(CDBSqlManager *pdb);

private:
	MyString m_sql;
	int m_result;
};

#endif _REGISTCARDDBDOOPERATOR_H__