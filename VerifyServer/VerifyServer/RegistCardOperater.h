#ifndef _REGISTCARDOPERATER_H__
#define _REGISTCARDOPERATER_H__

#include "operater.h"
#include "my_assist_define.h"

class CVIPCardManager;
class CClientManager;
class CRegistCardOperater :
	public COperater
{
public:
	CRegistCardOperater(CVIPCardManager *pmgr, 
		CClientManager *pclient_mgr,
		unsigned long socket_id,
		MyString strCardNumber, 
		MyString strCardPassword,
		MyString strCardPasswordEncode,
		MyString strCardUserName,
		MyString strCardUserPhone,
		MyString strCardUserFirstName,
		int nCardUserSex );
	virtual ~CRegistCardOperater(void);

protected:
	virtual bool _DoOperate(void);

private:
	CVIPCardManager *m_pmgr;
	CClientManager *m_pclient_mgr;
	unsigned long m_socket_id;
	MyString m_strCardNumber;
	MyString m_strCardPassword;
	MyString m_strCardPasswordEncode;
	MyString m_strCardUserName;
	MyString m_strCardUserPhone;
	int m_nCardUserSex;
	MyString m_strCardUserFirstName;
};

#endif