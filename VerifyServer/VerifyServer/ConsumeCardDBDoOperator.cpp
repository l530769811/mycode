#include "ConsumeCardDBDoOperator.h"
#include "DBSqlManager.h"
#include "error_id_define.h"

CConsumeCardDBDoOperator::CConsumeCardDBDoOperator(MyString strSql)
	: m_sql(strSql)
	, m_result(CARD_CONSUME_ERROR)
{
}


CConsumeCardDBDoOperator::~CConsumeCardDBDoOperator(void)
{
}


int CConsumeCardDBDoOperator::ConsumeCardCallback(void *data, int argc, char **argv, char **azColName)
{
	int nret = 0;
	if(argc>0)
	{

		{
			CConsumeCardDBDoOperator *pdata = reinterpret_cast<CConsumeCardDBDoOperator*>(data);
			pdata->m_result = NO_ERROR;
		}
	}
	return 0;

}

int CConsumeCardDBDoOperator::GetResult() const
{
	return m_result;
}

int CConsumeCardDBDoOperator::Exec(CDBSqlManager *pdb)
{
	int nret = -1;

	if(pdb!=0)
	{
		nret = pdb->ExecSql(m_sql.c_str(), ConsumeCardCallback, this);
		if (nret==0)
		{
			m_result = NO_ERROR;
		}
	}

	return nret;
}
