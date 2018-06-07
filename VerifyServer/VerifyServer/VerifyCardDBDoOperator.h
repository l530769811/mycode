#ifndef _VERIFYCARDDBDOOPERATOR_H__
#define _VERIFYCARDDBDOOPERATOR_H__

#include "dbdooperator.h"
#include "my_assist_define.h"

class CVerifyCardDBDoOperator :
	public CDBDoOperator
{
public:
	CVerifyCardDBDoOperator(MyString strSql);
	virtual ~CVerifyCardDBDoOperator(void);

	static int VerifyCardCallback(void *data, int argc, char **argv, char **azColName);
	int GetResult() const;
	MyString  GetResultText() const;

public:
	virtual int Exec(CDBSqlManager *pdb);

private:
	MyString m_sql;
	int m_result;
	MyString m_strResult;
};

#endif //_VERIFYCARDDBDOOPERATOR_H__