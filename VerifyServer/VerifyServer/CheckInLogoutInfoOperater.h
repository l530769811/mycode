#pragma once
#include "operater.h"
#include "my_assist_define.h"

class CClientManager;
class CCheckInLogoutInfoOperater :
	public COperater
{
public:
	CCheckInLogoutInfoOperater(CClientManager *pmgr, unsigned long nid, MyString strUserName, MyString strUserPassword, int result);
	virtual ~CCheckInLogoutInfoOperater(void);

protected:
	virtual bool _DoOperate(void);

private:
	CClientManager *m_client_mgr;
	unsigned long m_socket_id;
	MyString m_strUserName;
	MyString m_strUserPassword;
	int m_result;
};

