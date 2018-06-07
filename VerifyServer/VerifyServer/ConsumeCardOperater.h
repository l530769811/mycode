#ifndef _CONSUMECARDOPERATER_H__
#define _CONSUMECARDOPERATER_H__

#include "operater.h"
#include "my_assist_define.h"

class CVIPCardManager;
class CClientManager;
class CConsumeCardOperater :
	public COperater
{
public:
	CConsumeCardOperater(CVIPCardManager *pmgr, CClientManager *pclient_mgr, unsigned long socket_id, MyString strCardNumber, MyString strCardPassword);
	virtual ~CConsumeCardOperater(void);

protected:
	virtual bool _DoOperate(void);

private:
	CVIPCardManager *m_pmgr;
	CClientManager *m_pclient_mgr;
	unsigned long m_socket_id;

	MyString m_strCardNumber;
	MyString m_strCardPassword;
};

#endif _CONSUMECARDOPERATER_H__