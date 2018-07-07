#include "VerifyCardOperater.h"
#include "VIPCardManager.h"
#include "ClientManager.h"
#include "VerifyCardDBDoOperator.h"
#include "VerifyCardReponseNetSocketData.h"
#include "TcpServerNetSocketProxy.h"
#include "sqlite_sql.h"
#include "ProcessLogCollecter.h"
#include "error_id_define.h"

CVerifyCardOperater::CVerifyCardOperater(CVIPCardManager *pmgr, CClientManager *pclient_mgr, unsigned long socket_id, MyString strCardNumber, MyString strCardPassword)
	: m_pmgr(pmgr)
	, m_pclient_mgr(pclient_mgr)
	, m_socket_id(socket_id)
	, m_strCardNumber(strCardNumber)
	, m_strCardPassword(strCardPassword)
{
}


CVerifyCardOperater::~CVerifyCardOperater(void)
{
}

bool CVerifyCardOperater::_DoOperate()
{
	bool bret = false;
	if(m_pmgr!=0)
	{
		TCHAR sql[1024] = {0};
		_stprintf(sql, select_card_info_name_pass_encrypt, m_strCardNumber.c_str(), m_strCardPassword.c_str());
		MyString strSql = sql;
		CVerifyCardDBDoOperator op(strSql);
		m_pmgr->DoOperate(op);
		int result = op.GetResult();
		MyString strResult = op.GetResultText();

		MyString log_str;
		if(result==NO_ERROR)
		{			
			log_str = log_str + _T("Verify Card Number:") + m_strCardNumber + _T("    done");	
		}
		else
		{
			log_str = log_str + _T("Verify Card Number:") + m_strCardNumber + _T("    fail");	
		}
		log(TEXT_LOG, log_str);

		if(m_pclient_mgr!=0)
		{
			CVerifyCardReponseNetSocketData data(result, strResult, m_strCardNumber, m_strCardPassword);
			CTcpServerNetSocketProxy proxy(m_pclient_mgr, m_socket_id);
			m_pclient_mgr->ClientReponse(data, proxy);
		}
	}

	return bret;
}