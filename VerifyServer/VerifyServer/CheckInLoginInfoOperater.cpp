#include "CheckInLoginInfoOperater.h"
#include "ClientManager.h"
#include "error_id_define.h"
#include "TcpServerNetSocketProxy.h"
#include "LoginAffirmNetSocketData.h"

CCheckInLoginInfoOperater::CCheckInLoginInfoOperater(CClientManager *pmgr, unsigned long nid, MyString strUserName, MyString strUserPassword)
	: m_client_mgr(pmgr)
	, m_socket_id(nid)
	, m_strUserName(strUserName)
	, m_strUserPassword(strUserPassword)
{

}

CCheckInLoginInfoOperater::~CCheckInLoginInfoOperater(void)
{

}

bool CCheckInLoginInfoOperater::_DoOperate()
{
	bool bret = false;
	if (m_client_mgr!=0)
	{
		int result = m_client_mgr->AddConnectClient(m_socket_id, m_strUserName, m_strUserPassword);
		if (result!=0)
		{
			result = RELOGIN_USER_ERROR;
		}

		CTcpServerNetSocketProxy proxy(m_client_mgr, m_socket_id);
		CLoginAffirmNetSocketData data(result, m_strUserName, m_strUserPassword);
		m_client_mgr->ClientReponse(data, proxy);
	}
	return bret;
}