#ifndef _CHECKINLOGININFOOPERATER_H__
#define _CHECKINLOGININFOOPERATER_H__

#include "operater.h"
#include "my_assist_define.h"

class CClientManager;
class CCheckInLoginInfoOperater :
	public COperater
{
public:
	CCheckInLoginInfoOperater(CClientManager *pmgr, unsigned long nid, MyString strUserName, MyString strUserPassword);
	virtual ~CCheckInLoginInfoOperater(void);

protected:
	virtual bool _DoOperate(void);

private:
	CClientManager *m_client_mgr;
	unsigned long m_socket_id;
	MyString m_strUserName;
	MyString m_strUserPassword;

};

#endif //_CHECKINLOGININFOOPERATER_H__