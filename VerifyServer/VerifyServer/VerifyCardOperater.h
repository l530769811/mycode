#ifndef _VERIFYCARDOPERATER_H__
#define _VERIFYCARDOPERATER_H__

#include "operater.h"
#include "my_assist_define.h"

class CVIPCardManager;
class CClientManager;
class CVerifyCardOperater :
	public COperater
{
public:
	CVerifyCardOperater(CVIPCardManager *pmgr, CClientManager *pclient_mgr, unsigned long socket_id, MyString strCardNumber, MyString strCardPassword);
	virtual ~CVerifyCardOperater(void);

protected:
	virtual bool _DoOperate(void);

private:
	CVIPCardManager *m_pmgr;
	CClientManager *m_pclient_mgr;
	unsigned long m_socket_id;

	MyString m_strCardNumber;
	MyString m_strCardPassword;
};

#endif _VERIFYCARDOPERATER_H__