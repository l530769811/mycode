#ifndef _CLIENTSIGNUPDBDOOPERATOR_H__
#define _CLIENTSIGNUPDBDOOPERATOR_H__

#include "dbdooperator.h"
#include "my_assist_define.h"

class CClientSignupDBDoOperator :
	public CDBDoOperator
{
public:
	CClientSignupDBDoOperator(MyString sql);
	virtual ~CClientSignupDBDoOperator(void);

public:	
	virtual int Exec(CDBSqlManager *pdb);

private:
	MyString m_sql;
	int result;
};

#endif //_CLIENTSIGNUPDBDOOPERATOR_H__