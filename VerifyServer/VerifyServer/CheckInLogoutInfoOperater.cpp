#include "CheckInLogoutInfoOperater.h"
#include "ClientManager.h"
#include "error_id_define.h"

CCheckInLogoutInfoOperater::CCheckInLogoutInfoOperater(CClientManager *pmgr, unsigned long nid, MyString strUserName, MyString strUserPassword, int result)
	: m_client_mgr(pmgr)
	, m_socket_id(nid)
	, m_strUserName(strUserName)
	, m_strUserPassword(strUserPassword)
	, m_result(result)
{
}


CCheckInLogoutInfoOperater::~CCheckInLogoutInfoOperater(void)
{
}


bool CCheckInLogoutInfoOperater::_DoOperate()
{
	bool bret = false;
	if (m_client_mgr!=0)
	{
		if(m_result==NO_ERROR){
			int result = m_client_mgr->DeleteConnectClient(m_socket_id, m_strUserName, m_strUserPassword);
			if (result!=0)
			{
				result = RELOGIN_USER_ERROR;
			}
		}
		//m_client_mgr->ClientReponse(data, proxy);
	}
	return bret;
}