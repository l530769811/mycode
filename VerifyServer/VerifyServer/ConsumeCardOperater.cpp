#include "ConsumeCardOperater.h"
#include "VIPCardManager.h"
#include "ClientManager.h"
#include "ConsumeCardDBDoOperator.h"
#include "ConsumeCardReponseNetSocketData.h"
#include "TcpServerNetSocketProxy.h"
#include "sqlite_sql.h"

CConsumeCardOperater::CConsumeCardOperater(CVIPCardManager *pmgr, CClientManager *pclient_mgr, unsigned long socket_id, 
	MyString strCardNumber, MyString strCardPassword)
	: m_pmgr(pmgr)
	, m_pclient_mgr(pclient_mgr)
	, m_socket_id(socket_id)
	, m_strCardNumber(strCardNumber)
	, m_strCardPassword(strCardPassword)
{
}


CConsumeCardOperater::~CConsumeCardOperater(void)
{
}

bool CConsumeCardOperater::_DoOperate()
{
	bool bret = false;
	if(m_pmgr!=0)
	{
		TCHAR sql[1024] = {0};
		_stprintf(sql, select_card_info_name_pass_encrypt, m_strCardNumber.c_str(), m_strCardPassword.c_str());
		MyString strSql = sql;
		CConsumeCardDBDoOperator op(strSql);
		m_pmgr->DoOperate(op);
		int result = op.GetResult();
		if(m_pclient_mgr!=0)
		{
			CConsumeCardReponseNetSocketData data(result, m_strCardNumber, m_strCardPassword);
			CTcpServerNetSocketProxy proxy(m_pclient_mgr, m_socket_id);
			m_pclient_mgr->ClientReponse(data, proxy);
		}
	}

	return bret;
}