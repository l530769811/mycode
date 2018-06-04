#include "ClientSignupDBDoOperator.h"
#include "error_id_define.h"
#include "DBSqlManager.h"

CClientSignupDBDoOperator::CClientSignupDBDoOperator(MyString sql)
	: m_sql(sql)
	, result(SIGNUP_ERROR)
{
}


CClientSignupDBDoOperator::~CClientSignupDBDoOperator(void)
{
}

int CClientSignupDBDoOperator::Exec(CDBSqlManager *pdb)
{
	int ret = -1;
	if(pdb!=0)
	{
		ret= pdb->ExecSql(m_sql.c_str());
	}

	return ret;
}