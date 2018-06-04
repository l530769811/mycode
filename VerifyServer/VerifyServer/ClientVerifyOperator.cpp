#include "ClientVerifyOperator.h"
#include "ClientManager.h"
#include "LoginVerifyReponseNetSocketData.h"
#include "UdpNetSocketProxy.h"
#include "DBSqlManager.h"

CClientVerifyOperator::CClientVerifyOperator(MyString sql)
	: m_sql(sql)
	, result(false)
{
}


CClientVerifyOperator::~CClientVerifyOperator(void)
{
}


int CClientVerifyOperator::Exec(CDBSqlManager *pdb)
{
	int nret = -1;
	if(pdb!=0)
	{
		pdb->ExecSql(m_sql.c_str(), ClientVerifyCallback, this);
	}
	return nret;
}

bool CClientVerifyOperator::GetVerifyResult() const
{
	return result;
}

int CClientVerifyOperator::ClientVerifyCallback(void *data, int argc, char **argv, char **azColName)
{
	int ret = 0;

	//char *type_name = typeid(data).name;
	if(data!=0)
	{
		CClientVerifyOperator *pdata = reinterpret_cast <CClientVerifyOperator*>(data);
		pdata->result = (argc>0);
		
	}

	return ret;
}