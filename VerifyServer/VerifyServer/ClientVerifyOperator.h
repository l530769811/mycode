#ifndef _CLIENTVERIFTREPONSEOPERATER_H__
#define _CLIENTVERIFTREPONSEOPERATER_H__

#include "my_assist_define.h"
#include "MemAlloctor.h"
#include "DBDoOperator.h"


//class CUDPSocket;
//class CClientManager;
class CClientVerifyOperator :
	public CDBDoOperator, public CMemAlloctor
{
public:
	CClientVerifyOperator(MyString sql);
	virtual ~CClientVerifyOperator(void);

	static int ClientVerifyCallback(void *data, int argc, char **argv, char **azColName);
	bool GetVerifyResult() const;

public:	
	virtual int Exec(CDBSqlManager *pdb);

private:
	MyString m_sql;
	bool result;
};

#endif //_CLIENTVERIFTOPERATER_H__