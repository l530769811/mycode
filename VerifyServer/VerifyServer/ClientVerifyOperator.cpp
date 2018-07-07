#include "ClientVerifyOperator.h"
#include "ClientManager.h"
#include "LoginVerifyReponseNetSocketData.h"
#include "UdpNetSocketProxy.h"
#include "DBSqlManager.h"
#include "error_id_define.h"

CClientVerifyOperator::CClientVerifyOperator(MyString sql)
	: m_sql(sql)
	, result(LOGIN_VERIFY_ERROR)
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

int CClientVerifyOperator::GetVerifyResult() const
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
		pdata->result = (argc>0) ? NO_ERROR : LOGIN_VERIFY_ERROR;
		
	}

	return ret;
}