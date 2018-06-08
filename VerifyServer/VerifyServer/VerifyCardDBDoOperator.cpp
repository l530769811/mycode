#include "VerifyCardDBDoOperator.h"
#include "DBSqlManager.h"
#include "error_id_define.h"
#include <tchar.h>
#include "GlobalFunc.h"

CVerifyCardDBDoOperator::CVerifyCardDBDoOperator(MyString strSql)
	: m_sql(strSql)
	, m_result(CARD_VERIFY_ERROR)
	, m_strResult(_T(""))
{
}


CVerifyCardDBDoOperator::~CVerifyCardDBDoOperator(void)
{
}

int CVerifyCardDBDoOperator::VerifyCardCallback(void *data, int argc, char **argv, char **azColName)
{
	int nret = 0;
	if(argc>0)
	{
		
		{
			CVerifyCardDBDoOperator *pdata = reinterpret_cast<CVerifyCardDBDoOperator*>(data);
			pdata->m_result = NO_ERROR;
			int count_column = argc;
			TCHAR strResult[1024] = {0};
			for (int i=0; i<count_column; i++)
			{
				const int buff_len = 255;
				TCHAR	strColName[buff_len] = {0};
				TCHAR	strColValue[buff_len] = {0};

				GlobalUtf8ToUnicode(azColName[i], strColName, buff_len-1);
				GlobalUtf8ToUnicode(argv[i], strColValue, buff_len-1);
				
				pdata->m_strResult = pdata->m_strResult + strColName + _T(" : ") + strColValue + _T("\n");
			}			
		}
	}
	return 0;

}

int CVerifyCardDBDoOperator::GetResult() const
{
	return m_result;
}

MyString CVerifyCardDBDoOperator::GetResultText() const
{
	return m_strResult;
}

int CVerifyCardDBDoOperator::Exec(CDBSqlManager *pdb)
{
	int nret = -1;

	if(pdb!=0)
	{
		nret = pdb->ExecSql(m_sql.c_str(), VerifyCardCallback, this);
		
	}

	return nret;
}