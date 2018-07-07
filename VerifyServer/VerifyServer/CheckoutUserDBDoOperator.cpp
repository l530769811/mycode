#include "CheckoutUserDBDoOperator.h"
#include "error_id_define.h"
#include "DBSqlManager.h"
#include "sqlite_sql.h"

CCheckoutClientDBDoOperator::CCheckoutClientDBDoOperator(MyString strName)
	: m_strClientName(strName)
	, m_sql(_T(""))
	, result(LOGIN_USER_NO_SIGNUP_ERROR)
{
	TCHAR sql[MAX_PATH] = {0};
	_stprintf(sql, select_check_login_name, m_strClientName.c_str());
	m_sql = m_sql + sql;
}


CCheckoutClientDBDoOperator::~CCheckoutClientDBDoOperator(void)
{
}

int CCheckoutClientDBDoOperator::ClientCheckoutCallback(void *data, int argc, char **argv, char **azColName)
{
	int nret = -1;
	

	//char *type_name = typeid(data).name;
	if(data!=0)
	{
		CCheckoutClientDBDoOperator *pdata = reinterpret_cast <CCheckoutClientDBDoOperator*>(data);
		pdata->result = (argc>0) ? NO_ERROR : LOGIN_USER_NO_SIGNUP_ERROR;
	}

	
	return nret;
}

int CCheckoutClientDBDoOperator::GetCheckoutResult() const
{
	return result;
}

int CCheckoutClientDBDoOperator::Exec(CDBSqlManager *pdb)
{
	int nret = -1;
	if(pdb!=0)
	{
		pdb->ExecSql(m_sql.c_str(), ClientCheckoutCallback, this);
	}
	return nret;
}
