#ifndef _CHECKOUTCLIENTDBDOOPERATOR_H__
#define _CHECKOUTCLIENTDBDOOPERATOR_H__
#include "my_assist_define.h"
#include "dbdooperator.h"

class CCheckoutClientDBDoOperator :
	public CDBDoOperator
{
public:
	CCheckoutClientDBDoOperator(MyString strClientName);
	virtual ~CCheckoutClientDBDoOperator(void);

	static int ClientCheckoutCallback(void *data, int argc, char **argv, char **azColName);
	int GetCheckoutResult() const;

public:	
	virtual int Exec(CDBSqlManager *pdb);

private:
	MyString m_strClientName;
	MyString m_sql;
	int result;
};

#endif