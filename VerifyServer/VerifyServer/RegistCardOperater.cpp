#include "RegistCardOperater.h"
#include "VIPCardManager.h"
#include "ClientManager.h"
#include "RegistCardDBDoOperator.h"
#include "TcpServerNetSocketProxy.h"
#include "RegistCardReponseNetSocketData.h"
#include "sqlite_sql.h"

CRegistCardOperater::CRegistCardOperater(CVIPCardManager *pmgr,
	CClientManager *pclient_mgr,
	unsigned long socket_id,
	MyString strCardNumber, 
	MyString strCardPassword,
	MyString strCardPasswordEncode,
	MyString strCardUserName,
	MyString strCardUserPhone,
	MyString strCardUserFirstName,
	int nCardUserSex)
	: m_pmgr(pmgr)
	, m_pclient_mgr(pclient_mgr)
	, m_socket_id(socket_id)
	, m_strCardNumber(strCardNumber)
	, m_strCardPassword(strCardPassword)
	,m_strCardPasswordEncode(strCardPasswordEncode)
	, m_strCardUserName(strCardUserName)
	, m_strCardUserPhone(strCardUserPhone)
	, m_strCardUserFirstName(strCardUserFirstName)
	, m_nCardUserSex(nCardUserSex)
{
}


CRegistCardOperater::~CRegistCardOperater(void)
{
}

bool CRegistCardOperater::_DoOperate()
{
	bool bret = false;
	if(m_pmgr!=0)
	{
		TCHAR sql[1024] = {0};
		_stprintf(sql, insert_carduser_data, m_strCardNumber.c_str(),
			m_strCardPassword.c_str(),
			m_strCardPasswordEncode.c_str(),
			m_strCardUserFirstName.c_str(),
			_T(""),
			m_strCardUserName.c_str(),
			m_strCardUserPhone.c_str(), m_nCardUserSex, _T(""));
		MyString strSql = sql;
		CRegistCardDBDoOperator op(strSql);
		m_pmgr->DoOperate(op);
		int result = op.GetResult();
		if(m_pclient_mgr!=0)
		{
			CRegistCardReponseNetSocketData data(result, m_strCardNumber, m_strCardPassword);
			CTcpServerNetSocketProxy proxy(m_pclient_mgr, m_socket_id);
			m_pclient_mgr->ClientReponse(data, proxy);
		}
	}

	return bret;
}