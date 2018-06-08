#include "RegistCardDBDoOperator.h"
#include "DBSqlManager.h"
#include "sqlite_sql.h"
#include "error_id_define.h"

CRegistCardDBDoOperator::CRegistCardDBDoOperator(MyString strSql)
	: m_sql(strSql)
	, m_result(REGIST_ERROR)
{
}


CRegistCardDBDoOperator::~CRegistCardDBDoOperator(void)
{
}

int CRegistCardDBDoOperator::RegistCardCallback(void *data, int argc, char **argv, char **azColName)
{
	int n = -1;
	if(argc>0)
	{
		int i = argc;
	}
	return n;
}

int CRegistCardDBDoOperator::GetResult() const
{
	return m_result;
}

int CRegistCardDBDoOperator::Exec(CDBSqlManager *pdb)
{
	int nret = -1;
	if(pdb!=0)
	{
		nret =  pdb->ExecSql(m_sql.c_str(), RegistCardCallback, this);
		if (nret==0)
		{
			m_result = NO_ERROR;
		}		
	}
	return nret;
}